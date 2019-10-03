classdef PreProcessor < handle
  
  properties(SetAccess = private)
    rho_1;
    rho_2;
    coordinate_system;
    tolerance;
    mean;
    coef_variance;
    field_name;
    num_realizations;
  end
  
  methods
    
    function PreProcessorObject = PreProcessor(name,rho_1,rho_2,coord,tol,mean,CV,num_realizations)
      PreProcessorObject.field_name = name;
      PreProcessorObject.rho_1 = rho_1;
      PreProcessorObject.rho_2 = rho_2;
      PreProcessorObject.coordinate_system = coord;
      PreProcessorObject.tolerance = tol;
      PreProcessorObject.mean = mean;
      PreProcessorObject.coef_variance = CV;
      PreProcessorObject.num_realizations = num_realizations;
    end
    
  end
  
end
