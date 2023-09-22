#include "savedata.h"
//#include "biosig-dev.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>


#define T1T2

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#include "savedata_def.h"



#define min(a,b)        (((a) < (b)) ? (a) : (b))
#define max(a,b)        (((a) > (b)) ? (a) : (b))

#ifdef __cplusplus
extern "C" {
#endif
int savelink(const char* filename);
#ifdef __cplusplus
}
#endif

#ifdef WITH_PDP
void sopen_pdp_read(HDRTYPE *hdr);
#endif








#define GDF_VERSION_2_20 220

using namespace std;


SaveData::SaveData(QObject *parent)
    : QObject{parent}
{

}




/*
bool SaveData::saveToGDF(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file:" << fileName;
        return false;
    }

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream.writeRawData("BIOSEMI", 8);
    qint16 version = GDF_VERSION_2_20;
    stream << version;

        qint16 dataFormat = GDF_DATATYPE_INT16;
        stream.writeInt16(dataFormat);
        qint16 numChannelsShort = static_cast<qint16>(numChannels);
        stream.writeInt16(numChannelsShort);
        qint64 numSamplesPerRecordLong = static_cast<qint64>(numSamplesPerRecord);
        stream.writeInt64(numSamplesPerRecordLong);
        double samplingFrequencyDouble = samplingFrequency;
        stream.writeRawData(reinterpret_cast<const char*>(&samplingFrequencyDouble), sizeof(double));
        qint64 numDataRecordsLong = static_cast<qint64>(numDataRecords);
        stream.writeInt64(numDataRecordsLong);

        // Write channel information
        for (int i = 0; i < numChannels; i++) {
            qint16 channelType = GDF_CHANNEL_TYPE_EEG;
            qint16 physicalDimension = GDF_PHYSICAL_DIMENSION_MICROVOLT;
            qint16 prefiltering = 0;
            qint16 numSamplesPerRecordShort = static_cast<qint16>(numSamplesPerRecord);
            stream.writeInt16(channelType);
            stream.writeInt16(physicalDimension);
            stream.writeInt16(prefiltering);
            stream.writeInt16(numSamplesPerRecordShort);
            stream.writeRawData("Channel " + QString::number(i + 1).toLatin1(), 16);
            stream.writeRawData("Unknown", 80);
        }

        file.close();
        return true;
    }



*/




int SaveData::save2gdf(int argc, char **argv){

    //int argc = argv.size();
    HDRTYPE 	*hdr;
    size_t 	count, k1, ne=0;
    char 	*source, *dest, *tmpstr;
    biosig_options_type   biosig_options;
    biosig_options.free_text_event_limiter = "";	// default is no delimiter

    enum FileFormat SOURCE_TYPE; 		// type of file format
    struct {
    enum FileFormat TYPE;
    float VERSION;
    } TARGET;
    TARGET.TYPE=GDF;
    TARGET.VERSION=-1.0;

    int		COMPRESSION_LEVEL=0;
    int		status;
    uint16_t	k;
    uint16_t	chansel = 0;
    int 	TARGETSEGMENT=1; 	// select segment in multi-segment file format EEG1100 (Nihon Kohden)
    int 	VERBOSE	= 0;
    char	FLAG_ANON = 1;
    char	FLAG_CSV = 0;
    char	FLAG_JSON = 0;
    char	FLAG_DYGRAPH = 0;
    char	*argsweep = NULL;
    double	t1=0.0, t2=1.0/0.0;
#ifdef CHOLMOD_H
    char *rrFile = NULL;
    int   refarg = 0;
#endif

    for (k=1; k<argc; k++) {
        if (!strcmp(argv[k],"-v") || !strcmp(argv[k],"--version") ) {
        fprintf(stdout,"save2gdf (BioSig4C++) v%i.%i.%i\n", BIOSIG_VERSION_MAJOR, BIOSIG_VERSION_MINOR, BIOSIG_PATCHLEVEL);
        fprintf(stdout,"Copyright (C) 2006-2013 by Alois Schloegl and others\n");
        fprintf(stdout,"This file is part of BioSig http://biosig.sf.net - the free and\n");
        fprintf(stdout,"open source software library for biomedical signal processing.\n\n");
        fprintf(stdout,"BioSig is free software; you can redistribute it and/or modify\n");
        fprintf(stdout,"it under the terms of the GNU General Public License as published by\n");
        fprintf(stdout,"the Free Software Foundation; either version 3 of the License, or\n");
        fprintf(stdout,"(at your option) any later version.\n\n");
    }
        else if (!strcmp(argv[k],"-h") || !strcmp(argv[k],"--help") ) {
        fprintf(stdout,"\nusage: save2gdf [OPTIONS] SOURCE DEST\n");
        fprintf(stdout,"  SOURCE is the source file \n");
        fprintf(stdout,"      SOURCE can be also network file bscs://<hostname>/ID e.g. bscs://129.27.3.99/9aebcc5b4eef1024 \n");
        fprintf(stdout,"  DEST is the destination file \n");
        fprintf(stdout,"      DEST can be also network server bscs://<hostname>\n");
        fprintf(stdout,"      The corresponding ID number is reported and a bscs-link file is stored in /tmp/<SOURCE>.bscs\n");
        fprintf(stdout,"\n  Supported OPTIONS are:\n");
        fprintf(stdout,"   -v, --version\n\tprints version information\n");
        fprintf(stdout,"   -h, --help   \n\tprints this information\n");
#ifdef T1T2
        fprintf(stdout,"   [t0,dt]\n\tstart time and duration in seconds (do not use any spaces). \n");
        fprintf(stdout,"\tTHIS FEATURE IS CURRENTLY EXPERIMENTAL !!!\n");
#endif
#ifdef CHOLMOD_H
        fprintf(stdout,"   -r, --ref=MM  \n\trereference data with matrix file MM. \n\tMM must be a 'MatrixMarket matrix coordinate real general' file.\n");
#endif
        fprintf(stdout,"   -f=FMT  \n\tconverts data into format FMT\n");
        fprintf(stdout,"\tFMT must represent a valid target file format\n");
#if defined(WITH_SCP3)
        fprintf(stdout,"\tCurrently are supported: HL7aECG, SCP_ECG (EN1064:2005), SCP2, SCP3, GDF, EDF, BDF, CFWB, BIN, ASCII, ATF, BVA (BrainVision)\n\tas well as HEKA v2 -> ITX\n");
#else
        fprintf(stdout,"\tCurrently are supported: HL7aECG, SCP_ECG (EN1064:2005), GDF, EDF, BDF, CFWB, BIN, ASCII, ATF, BVA (BrainVision)\n\tas well as HEKA v2 -> ITX\n");
#endif
        fprintf(stdout, "   -a, --anon=yes   (default)\n"
                "\tanonymized data processing - personalize data (name, and birthday) is not processed but ignored.\n"
                "\tThe patient can be still identified with the unique patient identifier (and an external database).\n"
                "\tThis is for many cases sufficient (e.g. for research etc.). This mode can be turn off with\n"
                "   -n, --anon=no\n"
                "\tThis will process personal information like name and birthday. One might want to use this mode\n"
                "\twhen converting personalized patient data and no unique patient identifier is available.\n"
                "\tIt's recommended to pseudonize the data, or to use the patient identifier instead of patient name and birthday.\n"
        );
        fprintf(stdout,"   -CSV  \n\texports data into CSV file\n");
        fprintf(stdout,"   --free-text-event-limiter=\";\"\n\tfree text of events limited to first occurrence of \";\" (only EDF+/BDF+ format)\n");
        fprintf(stdout,"   -DYGRAPH, -f=DYGRAPH  \n\tproduces JSON output for presentation with dygraphs\n");
        fprintf(stdout,"   -JSON  \n\tshows header and events in JSON format\n");
        fprintf(stdout,"   -z=#, -z#\n\t# indicates the compression level (#=0 no compression; #=9 best compression, default #=1)\n");
        fprintf(stdout,"   -s=#\tselect target segment # (in the multisegment file format EEG1100)\n");
        fprintf(stdout,"   -SWEEP=ne,ng,ns\n\tsweep selection of HEKA/PM files\n\tne,ng, and ns select the number of experiment, the number of group, and the sweep number, resp.\n");
        fprintf(stdout,"   -VERBOSE=#, verbosity level #\n\t0=silent [default], 9=debugging\n");
        fprintf(stdout,"   --chan=CHAN\n\tselect channel CHAN (0: all channels, 1: first channel, etc.)\n");
        fprintf(stdout,"\n\n");
        return(0);
    }
        else if (!strncmp(argv[k],"-z",2))  	{
#ifdef ZLIB_H
        COMPRESSION_LEVEL = 1;	// default
        char *s = argv[k] + 2;
        if (s[0] == '=') s++;	// skip "="
        if (strlen(s)>0) COMPRESSION_LEVEL = atoi(s);
            if (COMPRESSION_LEVEL<0 || COMPRESSION_LEVEL>9)
            fprintf(stderr,"Error %s: Invalid Compression Level %s\n",argv[0],argv[k]);
#else
            fprintf(stderr,"Warning: option -z (compression) not supported. zlib not linked.\n");
#endif
    }
        else if (!strncmp(argv[k],"-VERBOSE",2))  {
            VERBOSE = argv[k][strlen(argv[k])-1]-48;
#ifndef NDEBUG
    // then VERBOSE_LEVEL is not a constant but a variable
    VERBOSE_LEVEL = VERBOSE;
#endif
    }
        else if (!strncasecmp(argv[k],"-SWEEP=",7))  {
            argsweep = argv[k]+6;

    }

    else if (!strcasecmp(argv[k],"-a") || !strcasecmp(argv[k],"--anon") )
        FLAG_ANON = 1;

    else if (!strcasecmp(argv[k],"-n") || !strcasecmp(argv[k],"--anon=no") )
        FLAG_ANON = 0;

    else if (!strcasecmp(argv[k],"-CSV"))
        FLAG_CSV = 1;

    else if (!strcasecmp(argv[k],"-JSON"))
        FLAG_JSON = 1;

    else if (!strcasecmp(argv[k],"-DYGRAPH"))
        FLAG_DYGRAPH = 1;

    else if (!strncmp(argv[k],"--free-text-event-limiter=",26))
        biosig_options.free_text_event_limiter = strstr(argv[k],"=") + 1;

        else if (!strncmp(argv[k],"-f=",3))  	{
            if (0) {}
            else if (!strncmp(argv[k],"-f=ASCII",8))
            TARGET.TYPE=ASCII;
        else if (!strcmp(argv[k],"-f=ATF"))
            TARGET.TYPE=ATF;
            else if (!strcmp(argv[k],"-f=BDF"))
            TARGET.TYPE=BDF;
            else if (!strncmp(argv[k],"-f=BIN",6))
            TARGET.TYPE=BIN;
            else if (!strncmp(argv[k],"-f=BVA",6))
            TARGET.TYPE=BrainVision;
            else if (!strncmp(argv[k],"-f=CFWB",7))
            TARGET.TYPE=CFWB;
            else if (!strcmp(argv[k],"-f=EDF"))
            TARGET.TYPE=EDF;
            else if (!strcmp(argv[k],"-f=GDF"))
            TARGET.TYPE=GDF;
        else if (!strcmp(argv[k],"-f=GDF1")) {
            TARGET.TYPE=GDF;
            TARGET.VERSION=1.0;
        }
        else if (!strcmp(argv[k],"-f=GDF2")) {
            TARGET.TYPE=GDF;
            TARGET.VERSION=2.0;
        }
#if (BIOSIG_VERSION >= 10700)
        else if (!strcmp(argv[k],"-f=GDF3")) {
            TARGET.TYPE=GDF;
            TARGET.VERSION=3.0;
        }
#endif
            else if (!strncmp(argv[k],"-f=HL7",6))
            TARGET.TYPE=HL7aECG;
        else if (!strncmp(argv[k],"-f=CSV",6)) {
            FLAG_CSV = 1;
        }
        else if (!strncmp(argv[k],"-f=DYGRAPH",10)) {
            FLAG_DYGRAPH = 1;
        }
            else if (!strncmp(argv[k],"-f=MFER",7))
            TARGET.TYPE=MFER;
#if defined(WITH_SCP3)
        else if (!strncmp(argv[k],"-f=SCP3",7) && (get_biosig_version() > 0x010806) ) {
            TARGET.TYPE=SCP_ECG;
            TARGET.VERSION=3.0;
            fprintf(stderr,"WARNING %s: Specification of SCPv3 is not finalized, "
                "\n\tand is subject to change without further notice. "
                "\n\tSCP3 may be used only for experimental work"
                "\n\tYou are warned !!!\n",__FILE__);
        }
#endif
        else if (!strncmp(argv[k],"-f=SCP",6)) {
            TARGET.TYPE=SCP_ECG;
            TARGET.VERSION=2.0;
        }
//    		else if (!strncmp(argv[k],"-f=TMSi",7))
//			TARGET.TYPE=TMSiLOG;
            else if (!strncmp(argv[k],"-f=ITX",6))
            TARGET.TYPE=ITX;
        else {
            fprintf(stderr,"format %s not supported.\n",argv[k]);
            return(-1);
        }
    }

#ifdef CHOLMOD_H
        else if ( !strncmp(argv[k],"-r=",3) || !strncmp(argv[k],"--ref=",6) )	{
                // re-referencing matrix
        refarg = k;
    }
#endif

        else if (!strncmp(argv[k],"-s=",3))  {
            TARGETSEGMENT = atoi(argv[k]+3);
    }

        else if (argv[k][0]=='[' && argv[k][strlen(argv[k])-1]==']' && (tmpstr=strchr(argv[k],',')) )  	{
        t1 = strtod(argv[k]+1,NULL);
        t2 = strtod(tmpstr+1,NULL);
        if (VERBOSE_LEVEL>7) fprintf(stdout,"[%f,%f]\n",t1,t2);
    }

    else {
        break;
    }

    if (VERBOSE_LEVEL>7)
        fprintf(stdout,"%s (line %i): save2gdf: arg%i = <%s>\n",__FILE__,__LINE__, k, argv[k]);

    }


    source = NULL;
    dest = NULL;

    switch (argc - k) {
    case 0:
        fprintf(stderr,"save2gdf: missing file argument\n");
        fprintf(stdout,"usage: save2gdf [options] SOURCE DEST\n");
        fprintf(stdout," for more details see also save2gdf --help \n");
        exit(-1);
        case 2:
        dest   = argv[k+1];
    case 1:
        source = argv[k];
        }

    if (VERBOSE_LEVEL<0) VERBOSE=1; // default
    if (VERBOSE_LEVEL>7) fprintf(stdout,"%s (line %i): SAVE2GDF %s %s started \n",__FILE__,__LINE__, source, dest);
    fprintf(stderr,"%s %s %s\n", argv[0], source, dest);

    tzset();
    hdr = constructHDR(0,0);
    // hdr->FLAG.OVERFLOWDETECTION = FlagOverflowDetection;
    hdr->FLAG.UCAL = ((TARGET.TYPE==BIN) || (TARGET.TYPE==SCP_ECG));
    hdr->FLAG.TARGETSEGMENT = TARGETSEGMENT;
    hdr->FLAG.ANONYMOUS = FLAG_ANON;

    if (argsweep) {
        k = 0;
        do {
    if (VERBOSE_LEVEL>7) fprintf(stdout,"SWEEP [109] %i: %s\t",k,argsweep);
            hdr->AS.SegSel[k++] = strtod(argsweep+1, &argsweep);
    if (VERBOSE_LEVEL>7) fprintf(stdout,",%i\n",hdr->AS.SegSel[k-1]);
        } while (argsweep[0]==',' && (k < 5) );
    }

    // HEKA2ITX hack
    if (TARGET.TYPE==ITX) {
        // hack: HEKA->ITX conversion will be done in SOPEN
        hdr->aECG = dest;
    }

    hdr = sopen_extended(source, "r", hdr, &biosig_options);
#ifdef WITH_PDP
    if (hdr->AS.B4C_ERRNUM) {
        biosigERROR(hdr, 0, NULL);  // reset error
        sopen_pdp_read(hdr);
    }
#endif
    // HEKA2ITX hack
        if (TARGET.TYPE==ITX) {
    if (hdr->TYPE==HEKA) {
        // hack: HEKA->ITX conversion is already done in SOPEN
        dest = NULL;
    }
    else {
                fprintf(stdout,"error: only HEKA->ITX is supported - source file is not HEKA file");
        biosigERROR(hdr, B4C_UNSPECIFIC_ERROR, "error: only HEKA->ITX is supported - source file is not HEKA file");
    }
    }

#ifdef CHOLMOD_H
    if (refarg > 0) {
                rrFile = strchr(argv[refarg], '=') + 1;
            if (RerefCHANNEL(hdr, rrFile, 1))
                    fprintf(stdout,"error: reading re-ref matrix %s \n",rrFile);
    }
#endif

    if (VERBOSE_LEVEL>7) fprintf(stdout,"%s (line %i): SOPEN-R finished (error %i)\n",__FILE__,__LINE__, hdr->AS.B4C_ERRNUM);

    if ((status=serror2(hdr))) {
        destructHDR(hdr);
        exit(status);
    }

    t1 *= hdr->SampleRate / hdr->SPR;
    t2 *= hdr->SampleRate / hdr->SPR;
    if (isnan(t1)) t1 = 0.0;
    if (t2+t1 > hdr->NRec) t2 = hdr->NRec - t1;

    if ( ( t1 - floor (t1) ) || ( t2 - floor(t2) ) ) {
        fprintf(stderr,"ERROR SAVE2GDF: cutting from parts of blocks not supported; t1 (%f) and t2 (%f) must be a multiple of block duration %f\n", t1,t2,hdr->SPR / hdr->SampleRate);
        biosigERROR(hdr, B4C_UNSPECIFIC_ERROR, "blocks must not be split");
    }

    if ((status=serror2(hdr))) {
        destructHDR(hdr);
        exit(status);
    }

    if (VERBOSE_LEVEL>7) fprintf(stdout,"%s (line %i) SOPEN-R finished\n",__FILE__,__LINE__);

    sort_eventtable(hdr);

    if (VERBOSE_LEVEL>7) fprintf(stdout,"%s (line %i) event table sorted\n",__FILE__,__LINE__);

    if (FLAG_JSON) {
        fprintf_hdr2json(stdout, hdr);
    }
    else {
        hdr2ascii(hdr, stdout, VERBOSE);
    }

    // all channels are converted - channel selection currently not supported
        for (k=0; k<hdr->NS; k++) {
        if ( (hdr->CHANNEL[k].OnOff > 0) && hdr->CHANNEL[k].SPR ) {
            if ((hdr->SPR/hdr->CHANNEL[k].SPR)*hdr->CHANNEL[k].SPR != hdr->SPR)
                 fprintf(stdout,"Warning: channel %i might be decimated!\n",k+1);
            };
            // hdr->CHANNEL[k].OnOff = 1;	// convert all channels
        }

#ifdef CHOLMOD_H
        int flagREREF = hdr->Calib != NULL && hdr->rerefCHANNEL != NULL;
#else
        int flagREREF = 0;
#endif
    hdr->FLAG.OVERFLOWDETECTION = 0;
    hdr->FLAG.UCAL = hdr->FLAG.UCAL && !flagREREF;
    hdr->FLAG.ROW_BASED_CHANNELS = flagREREF;

#ifdef CHOLMOD_H
    if (VERBOSE_LEVEL>7)
        fprintf(stdout,"%s (line %i): %p %p Flag.ReRef=%i\n",__FILE__,__LINE__,hdr->Calib, hdr->rerefCHANNEL,flagREREF);
#endif

    if (VERBOSE_LEVEL>7)
        fprintf(stdout,"%s (line %i): SREAD [%f,%f].\n",__FILE__,__LINE__,t1,t2);

    if (hdr->NRec <= 0) {
        // in case number of samples is not known
        count = sread(NULL, t1, (size_t)-1, hdr);
        t2 = count;
    }
    else {
        if (t2+t1 > hdr->NRec) t2 = hdr->NRec - t1;
        if ((dest != NULL) || FLAG_CSV || FLAG_DYGRAPH )
            count = sread(NULL, t1, t2, hdr);
    }

    biosig_data_type* data = hdr->data.block;
    if ((VERBOSE_LEVEL>8) && (hdr->data.size[0]*hdr->data.size[1]>500))
        fprintf(stdout,"%s (line %i): UCAL=%i %e %e %e \n",__FILE__,__LINE__,hdr->FLAG.UCAL,data[100],data[110],data[500+hdr->SPR]);

    if ((status=serror2(hdr))) {
        destructHDR(hdr);
        exit(status);
    };

    if (VERBOSE_LEVEL>7)
        fprintf(stdout,"\n%s (line %i): SREAD on %s successful [%i,%i].\n",__FILE__,__LINE__,hdr->FileName,(int)hdr->data.size[0],(int)hdr->data.size[1]);

//	fprintf(stdout,"\n %f,%f.\n",hdr->FileName,hdr->data.block[3*hdr->SPR],hdr->data.block[4*hdr->SPR]);
    if (VERBOSE_LEVEL>7)
        fprintf(stdout,"\n%s (line %i): File  %s =%i/%i\n",__FILE__,__LINE__,hdr->FileName,hdr->FILE.OPEN,hdr->FILE.Des);

    if ((dest==NULL) && !FLAG_CSV && !FLAG_DYGRAPH) {
        if (ne)	/* used for testig SFLUSH_GDF_EVENT_TABLE */
        {
            if (hdr->EVENT.N > ne)
                hdr->EVENT.N -= ne;
            else
                hdr->EVENT.N  = 0;

            // fprintf(stdout,"Status-SFLUSH %i\n",sflush_gdf_event_table(hdr));
        }

        if (VERBOSE_LEVEL>7) fprintf(stdout,"%s (line %i): going for SCLOSE\n",__FILE__,__LINE__);
        sclose(hdr);
        if (VERBOSE_LEVEL>7) fprintf(stdout,"%s (line %i): SCLOSE(HDR) finished\n",__FILE__,__LINE__);
        status=serror2(hdr);
        destructHDR(hdr);
        exit(status);
    }

    if (hdr->FILE.OPEN) {
        sclose(hdr);
        free(hdr->AS.Header);
        hdr->AS.Header = NULL;
        if (VERBOSE_LEVEL>7) fprintf(stdout,"%s (line %i): file closed\n",__FILE__,__LINE__);
    }
    if (VERBOSE_LEVEL>7 )
        fprintf(stdout,"\n%s (line %i): File %s closed sd=%i/%i\n",__FILE__,__LINE__,hdr->FileName,hdr->FILE.OPEN,hdr->FILE.Des);

    SOURCE_TYPE = hdr->TYPE;
    if (FLAG_DYGRAPH) TARGET.TYPE=SOURCE_TYPE;

    hdr->TYPE = TARGET.TYPE;
    if (TARGET.VERSION>=0) hdr->VERSION = TARGET.VERSION;

    hdr->FILE.COMPRESSION = COMPRESSION_LEVEL;

   /*******************************************
    make block size as small as possible
    *******************************************/

    if (1) {
        uint32_t asGCD=hdr->SPR;
            for (k=0; k<hdr->NS; k++)
                if (hdr->CHANNEL[k].OnOff && hdr->CHANNEL[k].SPR)
                asGCD = gcd(asGCD, hdr->CHANNEL[k].SPR);
        if (TARGET.TYPE==EDF) {
            double d = asGCD / hdr->SampleRate;
            if (d==ceil(d)) asGCD = d; 	// make block duration 1 second
        }
            hdr->SPR  /= asGCD;
            hdr->NRec *= asGCD;
            for (k=0; k<hdr->NS; k++)
                hdr->CHANNEL[k].SPR /= asGCD;
#ifdef CHOLMOD_H
        if (hdr->Calib)
                for (k=0; k<hdr->Calib->ncol; k++)
                    hdr->rerefCHANNEL[k].SPR /= asGCD;
#endif
        }

   /*********************************
    re-referencing
    *********************************/

#ifdef CHOLMOD_H
    if (VERBOSE_LEVEL>7) fprintf(stdout,"%s (line %i): %p %p\n",__FILE__,__LINE__,hdr->CHANNEL,hdr->rerefCHANNEL);

        if (hdr->Calib && hdr->rerefCHANNEL) {
            if (VERBOSE_LEVEL>6)
                    hdr2ascii(hdr,stdout,3);

        hdr->NS = hdr->Calib->ncol;

                free(hdr->CHANNEL);
                hdr->CHANNEL = hdr->rerefCHANNEL;
                hdr->rerefCHANNEL = NULL;

    if (VERBOSE_LEVEL>7) fprintf(stdout,"[200-]\n");

        RerefCHANNEL(hdr, NULL, 0);	// clear HDR.Calib und HDR.rerefCHANNEL

    if (VERBOSE_LEVEL>7) fprintf(stdout,"[200+]\n");
                hdr->Calib = NULL;

            if (VERBOSE_LEVEL>6)
                    hdr2ascii(hdr,stdout,3);
        }
#endif

   /*********************************
    Write data
   *********************************/

    //************ identify Max/Min **********

    if (VERBOSE_LEVEL>7) fprintf(stdout,"[201]\n");

    double PhysMaxValue0 = -INFINITY; //hdr->data.block[0];
    double PhysMinValue0 = +INFINITY; //hdr->data.block[0];
    biosig_data_type val = NAN;
    char FLAG_CONVERSION_TESTED = 1;
    size_t N;
#ifdef T1T2
    N = hdr->FLAG.ROW_BASED_CHANNELS ? hdr->data.size[1] : hdr->data.size[0];
    hdr->NRec = N/hdr->SPR;
#else
    N = hdr->NRec*hdr->SPR;
#endif
    typeof(hdr->NS) k2=0;
        for (k=0; k<hdr->NS; k++)
        if (hdr->CHANNEL[k].OnOff && hdr->CHANNEL[k].SPR) {

        if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i): #%i %i %i N=%i [%i,%i]\n",__FILE__,__LINE__,(int)k,(int)k2,(int)hdr->FLAG.ROW_BASED_CHANNELS,(int)N,(int)hdr->data.size[0],(int)(hdr->data.size[1]));

        double MaxValue;
        double MinValue;
        double MaxValueF;
        double MinValueF;
        double MaxValueD;
        double MinValueD;
        if (hdr->FLAG.ROW_BASED_CHANNELS) {
            MaxValue = hdr->data.block[k2];
            MinValue = hdr->data.block[k2];
            for (k1=1; k1<N; k1++) {
                val = hdr->data.block[k2 + k1*hdr->data.size[0]];
                if (MaxValue < val) MaxValue = val;
                if (MinValue > val) MinValue = val;
            }
        }
        else {
            MaxValue = hdr->data.block[k2*N];
            MinValue = hdr->data.block[k2*N];
            for (k1=1; k1<N; k1++) {
                val = hdr->data.block[k2*N + k1];
                if (MaxValue < val) MaxValue = val;
                if (MinValue > val) MinValue = val;
            }
        }

        if (!hdr->FLAG.UCAL) {
            MaxValueF = MaxValue;
            MinValueF = MinValue;
            MaxValueD = (MaxValue - hdr->CHANNEL[k].Off) / hdr->CHANNEL[k].Cal;
            MinValueD = (MinValue - hdr->CHANNEL[k].Off) / hdr->CHANNEL[k].Cal;
        }
        else {
            MaxValueF = MaxValue * hdr->CHANNEL[k].Cal + hdr->CHANNEL[k].Off;
            MinValueF = MinValue * hdr->CHANNEL[k].Cal + hdr->CHANNEL[k].Off;
            MaxValueD = MaxValue;
            MinValueD = MinValue;
        }
        if (PhysMaxValue0 < MaxValueF) PhysMaxValue0 = MaxValueF;
        if (PhysMinValue0 > MinValueF) PhysMinValue0 = MinValueF;

        if ((SOURCE_TYPE==alpha) && (hdr->CHANNEL[k].GDFTYP==(255+12)) && (TARGET.TYPE==GDF))
            // 12 bit into 16 bit
            ; //hdr->CHANNEL[k].GDFTYP = 3;
        else if ((SOURCE_TYPE==ETG4000) && (TARGET.TYPE==GDF)) {
            hdr->CHANNEL[k].GDFTYP  = 16;
            hdr->CHANNEL[k].PhysMax = MaxValueF;
            hdr->CHANNEL[k].PhysMin = MinValueF;
            hdr->CHANNEL[k].DigMax  = MaxValueD;
            hdr->CHANNEL[k].DigMin  = MinValueD;
        }
/* TODO: check whether this is really needed - was probably just a workaround for another bug

        else if ((SOURCE_TYPE==GDF) && (TARGET.TYPE==GDF))
            ;
        else if (SOURCE_TYPE==HEKA)
            ;
        else if (SOURCE_TYPE==ABF)
            ;
*/
        else if (TARGET.TYPE==SCP_ECG && !hdr->FLAG.UCAL) {
            double scale = PhysDimScale(hdr->CHANNEL[k].PhysDimCode) *1e9;
            if (hdr->FLAG.ROW_BASED_CHANNELS) {
                for (k1=0; k1<N; k1++)
                    hdr->data.block[k2 + k1*hdr->data.size[0]] *= scale;
            }
            else {
                for (k1=0; k1<N; k1++)
                    hdr->data.block[k2*N + k1] *= scale;
            }
                hdr->CHANNEL[k].GDFTYP = 3;
                hdr->CHANNEL[k].PhysDimCode = 4276;	// nV
                hdr->CHANNEL[k].DigMax = ldexp(1.0,15)-1.0;
                hdr->CHANNEL[k].DigMin = -hdr->CHANNEL[k].DigMax;
            double PhysMax = max(fabs(PhysMaxValue0),fabs(PhysMinValue0)) * scale;
                hdr->CHANNEL[k].PhysMax = PhysMax;
                hdr->CHANNEL[k].PhysMin = -PhysMax;
        }
        else if (TARGET.TYPE==EDF) {
                hdr->CHANNEL[k].GDFTYP = 3;
                hdr->CHANNEL[k].DigMax = ldexp(1.0,15)-1.0;
                hdr->CHANNEL[k].DigMin = -hdr->CHANNEL[k].DigMax;
            hdr->CHANNEL[k].PhysMax = MaxValueF;
            hdr->CHANNEL[k].PhysMin = MinValueF;
                hdr->CHANNEL[k].Cal = (hdr->CHANNEL[k].PhysMax - hdr->CHANNEL[k].PhysMin) / (hdr->CHANNEL[k].DigMax - hdr->CHANNEL[k].DigMin);
                hdr->CHANNEL[k].Off = hdr->CHANNEL[k].PhysMin - hdr->CHANNEL[k].DigMin * hdr->CHANNEL[k].Cal;
        }
        else if ((SOURCE_TYPE==EDF) && (TARGET.TYPE==GDF)) {
            // do nothing
        }
        else if ((hdr->CHANNEL[k].GDFTYP<10 ) && (TARGET.TYPE==GDF || TARGET.TYPE==CFWB)) {
            /* heuristic to determine optimal data type */

            if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i): #%i %i %f %f %f %f %f %f\n",__FILE__,__LINE__,k+1,(int)hdr->FLAG.UCAL,MinValue,MaxValue,MinValueF,MaxValueF,MinValueD,MaxValueD);

            if ((MaxValueD <= 127) && (MinValueD >= -128))
                    hdr->CHANNEL[k].GDFTYP = 1;
            else if ((MaxValueD <= 255.0) && (MinValueD >= 0.0))
                    hdr->CHANNEL[k].GDFTYP = 2;
            else if ((MaxValueD <= ldexp(1.0,15)-1.0) && (MinValueD >= ldexp(-1.0,15)))
                    hdr->CHANNEL[k].GDFTYP = 3;
            else if ((MaxValueD <= ldexp(1.0,16)-1.0) && (MinValueD >= 0.0))
                    hdr->CHANNEL[k].GDFTYP = 4;
            else if ((MaxValueD <= ldexp(1.0,31)-1.0) && (MinValueD >= ldexp(-1.0,31)))
                    hdr->CHANNEL[k].GDFTYP = 5;
            else if ((MaxValueD <= ldexp(1.0,32)-1.0) && (MinValueD >= 0.0))
                    hdr->CHANNEL[k].GDFTYP = 6;
        }
        else {
            FLAG_CONVERSION_TESTED = 0;
        }

        if (VERBOSE_LEVEL>7) fprintf(stdout,"#%3d %d [%g %g][%g %g]\n",k,hdr->CHANNEL[k].GDFTYP,MinValue,MaxValue,PhysMinValue0,PhysMaxValue0);
        k2++;
    }
    if (!FLAG_CONVERSION_TESTED)
        fprintf(stderr,"Warning SAVE2GDF: conversion from %s to %s not tested\n",GetFileTypeString(SOURCE_TYPE),GetFileTypeString(TARGET.TYPE));

    if (VERBOSE_LEVEL>7) fprintf(stdout,"%s (line %i): UCAL=%i\n",__FILE__,__LINE__, hdr->FLAG.UCAL);

    hdr->FLAG.ANONYMOUS = FLAG_ANON;

    /*
      keep header data from previous file, in might contain optional data
      (GDF Header3, EventDescription, hdr->SCP.SectionX, etc. )
      and might still be referenced and needed.
    */
    void *tmpmem = hdr->AS.Header;
    hdr->AS.Header = NULL;

    if (FLAG_CSV) {
        const char SEP=',';
        FILE *fid = stdout;
        if (dest != NULL) fid = fopen(dest,"wt+");
        ssize_t k1;
        size_t k2;
        char flag = 0;
        for (k2 = 0; k2 < hdr->NS; k2++) {
            CHANNEL_TYPE *hc = hdr->CHANNEL+k2;
            if (hc->OnOff) {
                if (flag) fprintf(fid,"%c",SEP);
                flag = 1;
                fprintf(fid,"\"%s [%s]\"", hc->Label, PhysDim3(hc->PhysDimCode) );
            }
        }
        for (k1 = 0; k1 < hdr->SPR*hdr->NRec; k1++) {
            flag = 0;
            for (k2 = 0; k2 < hdr->NS; k2++) {
                CHANNEL_TYPE *hc = hdr->CHANNEL+k2;
                if (hc->OnOff) {
                    fprintf(fid,"%c", flag ? SEP : '\n');
                    flag = 1;
                    size_t p = hdr->FLAG.ROW_BASED_CHANNELS ? hdr->data.size[0] * k1 + k2 : hdr->data.size[0] * k2 + k1 ;
                    fprintf(fid,"%g", data[p]);
                }
            }
        }
        fprintf(fid,"\n");
        if (dest != NULL) fclose(fid);
    }
    else if (FLAG_DYGRAPH) {
        FILE *fid = stdout;
        if (dest != NULL) fid = fopen(dest,"wb+");
        fprintf(fid,"{\n\"Header\": ");
        fprintf_hdr2json(fid, hdr);
        fprintf(fid,",\n\"Data\": [ ");
        ssize_t k1;
        size_t k2;
        for (k1=0; k1 < hdr->SPR * hdr->NRec; k1++) {
            if (k1>0) fprintf(fid,",\n\t");
            fprintf(fid,"[");
            for (k2=0; k2<hdr->NS; k2++) {
                if (k2>0) fprintf(fid,", ");
                size_t p = hdr->FLAG.ROW_BASED_CHANNELS ? hdr->data.size[0] * k1 + k2 : hdr->data.size[0] * k2 + k1 ;
                fprintf(fid,"%g",data[p]);
            }
            fprintf(fid,"]");
        }

        fprintf(fid," ],\n\"labels\": [ ");
        for (k2=0; k2<hdr->NS; k2++) {
            CHANNEL_TYPE *hc = hdr->CHANNEL+k2;
            if (k2>0) fprintf(fid,", ");
            fprintf(fid,"\"%s [%s]\"",hc->Label,PhysDim3(hc->PhysDimCode));
        }
        fprintf(fid,"]\n}\n");
        if (dest != NULL) fclose(fid);
    }
    else {
        /* write file */
        size_t destlen = strlen(dest);
        char *tmp = (char*)malloc(destlen+4);
        strcpy(tmp,dest);
        if (hdr->FILE.COMPRESSION)  // add .gz extension to filename
        strcpy(tmp+destlen,".gz");

    if (VERBOSE_LEVEL>7)
        fprintf(stdout,"%s (line %i) z=%i sd=%i\n",__FILE__,__LINE__,hdr->FILE.COMPRESSION,hdr->FILE.Des);

        sopen(tmp, "wb", hdr);

    if (VERBOSE_LEVEL>7) fprintf(stdout,"returned from sopen-wb\n");

        free(tmp);
        if ((status=serror2(hdr))) {
            destructHDR(hdr);
            exit(status);
        }
#ifndef WITHOUT_NETWORK
        if (hdr->FILE.Des>0)
            savelink(source);
#endif
        if (VERBOSE_LEVEL>7)
            fprintf(stdout,"\n%s (line %i): File %s opened. %i %i %i Des=%i\n",__FILE__,__LINE__,hdr->FileName,hdr->AS.bpb,hdr->NS,(int)(hdr->NRec),hdr->FILE.Des);

        swrite(data, hdr->NRec, hdr);

        if (VERBOSE_LEVEL>7) fprintf(stdout,"%s (line %i): SWRITE finishes (errno=%i)\n",__FILE__,__LINE__,(int)hdr->AS.B4C_ERRNUM);
        if ((status=serror2(hdr))) {
            destructHDR(hdr);
            exit(status);
            }

        if (VERBOSE_LEVEL>7) fprintf(stdout,"%s (line %i): SWRITE finishes %i\n",__FILE__,__LINE__,(int)status);

        sclose(hdr);

        if (VERBOSE_LEVEL>7) fprintf(stdout,"%s (line %i): SCLOSE finished\n",__FILE__,__LINE__);

    }
    status = serror2(hdr);
    destructHDR(hdr);
    if (tmpmem != NULL) free(tmpmem);
    exit(status);
}







