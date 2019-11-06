classdef Field < handle
  
  properties
    rho_1;
    rho_2;
    coordinate_system;
    e1;
    e2;
    tolerance;
    mean;
    coef_variance;
    name;
    d;
    v;
    seed = -1;
  end
  
  methods
    
    function FieldObject = Field(name,rho_1,rho_2,...
        coord,e1,e2,tol,mean,CV)
      FieldObject.name = name;
      FieldObject.rho_1 = rho_1;
      FieldObject.rho_2 = rho_2;
      FieldObject.coordinate_system = coord;
      FieldObject.e1 = e1;
      FieldObject.e2 = e2;
      FieldObject.tolerance = tol;
      FieldObject.mean = mean;
      FieldObject.coef_variance = CV;
    end
    
    function G = sampleGaussian(this)
      nu = length(this.d);
      if this.seed >= 0
        rng(this.seed);
      end
      xi = randn(nu,1);
      G = this.v*(sqrt(this.d).*xi);
    end
    
  end
  
end
