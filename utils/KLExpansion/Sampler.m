classdef Sampler
  
  properties
    num_samples;
    field_correlation;
    field_1;
    field_2;
    X;
    Y;
    same_kernel;
  end
  
  methods
    
    function this = Sampler(n,rho,f1,f2,Xmesh,Ymesh,same_kernel)
      this.field_1 = f1;
      this.field_2 = f2;
      this.field_correlation = rho;
      this.num_samples = n;
      this.X = Xmesh;
      this.Y = Ymesh;
      this.same_kernel = same_kernel;
    end
    
    function this = run(this)
      fprintf('Solving the first field\n');
      this.solveField(this.field_1);
      fprintf('Solving the second field\n');
      if this.same_kernel
        this.field_2.d = this.field_1.d;
        this.field_2.v = this.field_1.v;
      else
        this.solveField(this.field_2);
      end
      
      this.sample();
    end
    
    function this = solveField(this,field)
      [field.d,field.v] = KLexpansion(field,this.X,this.Y);
    end
    
    function this = sample(this)
      for sample = 1:this.num_samples
        G1 = this.field_1.sampleGaussian();
        G2 = this.field_2.sampleGaussian();
        for rho = this.field_correlation
          field1 = GaussianToGamma(this.field_1.mean,this.field_1.coef_variance,G1);
          field2 = GaussianToGamma(this.field_2.mean,this.field_2.coef_variance,...
            rho*G1+sqrt(1-rho^2)*G2);
          % Preprocess to match MOOSE syntax
          [Nx,Ny] = size(this.X);
          field1 = reshape(field1,Ny,Nx);
          field1 = field1';
          field2 = reshape(field2,Ny,Nx);
          field2 = field2';
          if sample == 1
            this.plotField([this.field_1.name,' rho = ',num2str(rho)],field1);
            this.plotField([this.field_2.name,' rho = ',num2str(rho)],field2);
          end
          fprintf('%s: [%.3E, %.3E]\n',this.field_1.name,min(field1(:)),max(field1(:)));
          fprintf('%s: [%.3E, %.3E]\n',this.field_2.name,min(field2(:)),max(field2(:)));
          this.writeToTXT([this.field_1.name,'_rho_',num2str(rho),'_sample_',num2str(sample),'.txt'],field1);
          this.writeToTXT([this.field_2.name,'_rho_',num2str(rho),'_sample_',num2str(sample),'.txt'],field2);
        end
      end
    end
    
    function plotField(this,name,field)
      figure
      surf(this.X,this.Y,field,'EdgeColor','none');
      view(2);
      axis equal
      axis off
      title(name)
      colorbar
    end
    
    function writeToTXT(this,file_name,field)
      fprintf('writing field %s\n',file_name);
      fileID = fopen(file_name,'wt');
      
      [Nx,Ny] = size(field);
      Np = length(field(:));
      
      % write X coordinates
      fprintf(fileID,'AXIS X\n');
      for i = 1:Nx-1
        fprintf(fileID,'%E ',this.X(1,i));
      end
      fprintf(fileID,'%E\n',this.X(1,Nx));
      
      % write Y coordinates
      fprintf(fileID,'AXIS Y\n');
      for i = 1:Ny-1
        fprintf(fileID,'%E ',this.Y(i,1));
      end
      fprintf(fileID,'%E\n',this.Y(Ny,1));
      
      % write normalized eigenvector
      fprintf(fileID,'DATA\n');
      for i = 1:Np-1
        fprintf(fileID,'%E ',field(i));
      end
      fprintf(fileID,'%E\n',field(Np));
      
      fclose(fileID);
    end
    
  end
  
end

