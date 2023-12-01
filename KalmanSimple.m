classdef KalmanSimple < handle
   properties
      x (2,1)
      x_ (2,1)
      Phi (2,2) 
      H (1,2) 
      f0
      A
      P (2,2)
      P_(2,2)
      res
      S
      K
      
      Q (2,2) % Add Q as a property
      R (1,1) % Add R as a property
      
      
   end
   methods
      function obj = KalmanSimple(f0,A,srate,q,r)
          
            obj.f0 = f0;
            obj.A = A;
            obj.H(1,:) = [1,0];
            obj.Phi = [cos(2*pi*f0/srate),-sin(2*pi*f0/srate);
                                                sin(2*pi*f0/srate),cos(2*pi*f0/srate)];
            
            
            obj.x(:,1) = [0,0];
            obj.x_(:,1) = [0,0];
            obj.P(:,:) = eye(2);
            
            obj.P_(:,:) = eye(2);
            
            obj.Q(:,:) = eye(2)*q; % Initialize Q
            obj.R(:,:) = eye(1)*r; % Initialize R
      
         end
         
         
    
       
       
       
       
      function filtered = apply(obj, raw)
      
         filtered = zeros(size(raw));
         for i = 1:size(raw,1)
             filtered(i) = obj.step(raw(i));
         end
      
        
         plot(filtered)
         
      end
     
      function filtered_value = step(obj, y) 
        obj.x_ = obj.Phi*obj.x;
        oo = obj.x;
        
        
        obj.P_ = obj.Phi*obj.P*obj.Phi'+obj.Q;
        
        obj.res = y-obj.H*obj.x_;
        
        obj.S = obj.H*obj.P_*obj.H'+obj.R;
        
        obj.K = (obj.P_*obj.H')/obj.S;
        
        obj.x = obj.x_+obj.K*obj.res;

        
        obj.P = (eye(2)-obj.K*obj.H)*obj.P_;
        filtered_value = obj.x(1,1);
      end
   end
end