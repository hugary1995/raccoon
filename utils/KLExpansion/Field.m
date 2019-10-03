classdef Field < handle
  
  properties
    rho_1;
    rho_2;
    coordinate_system;
    tolerance;
    mean;
    coef_variance;
    name;
    d;
    v;
  end
  
  methods
    
    function FieldObject = Field(name,rho_1,rho_2,...
        coord,tol,mean,CV)
      FieldObject.name = name;
      FieldObject.rho_1 = rho_1;
      FieldObject.rho_2 = rho_2;
      FieldObject.coordinate_system = coord;
      FieldObject.tolerance = tol;
      FieldObject.mean = mean;
      FieldObject.coef_variance = CV;
    end
    
    function G = sampleGaussian(this)
      nu = length(this.d);
      xi = randn(nu,1);
      G = this.v*(sqrt(this.d).*xi);
    end
    
  end
  
end
