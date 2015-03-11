classdef mvec < handle
% MVEC create a Geometric Algebra multi-vector
%
%   mvec without argument returns a 0 scalar multi-vector
%
%   mvec(x) where x is a scalar value returns an x scalar multi-vector 
%
%   mvec(x) where x is a vector returns a pure multi-vector
%
%   Examples:
%
%   mvec([0 1]) creates a multi-vector: 1 e2
%   mvec([3 2 4]) creates a multi-vector: 3 e1 +2 e2 +4 e3
%   mvec(2.5) creates a multi-vector: 2.5
%
%   a = mvec([3 2]);
%   b = mvec([-1 1]);
%   a.*b - gives inner product
%   a^b - gives outer product
%   a*b = a.*b + a^b - gives geometric product
%   a/b - divides multi-vectors
%   a+b - adds multi-vectors
%   a-b - subtracts multi-vectors
%   a.grade(x) - returns a pure multi-vector with blades of grade x
%   a.vec(dim) - converts multi-vector to vector of dimenison dim
%   a.scalar - return scalar part of multi-vector

   properties (Hidden)

   end
   % The following properties can be set only by class methods
   properties (SetAccess = private)
      gcamdata=[];
   end
   methods
      % Create a new multivector
      function m = mvec(x)
         % If no arguments passed in, create a zero-scalara vector
         if(~exist('x','var'))
             m.gcamdata = gcamvec('n',0);          
         elseif(ischar(x))
             m.gcamdata = gcamvec('n',x);
         elseif(iscell(x))
             m.gcamdata = x; 
         else
             m.gcamdata = gcamvec('n',double(x));
         end
         % Calling a static method requires the class name
         % addAccount registers the InsufficientFunds listener on this instance
      end

      function c = times(a,b)
         c = mvec(gcamvec('&',a,b));
         %if(length(c.gcamdata)==1)
         %  c = blade(c.gcamdata{1});
         %end
      end

      function c = mpower(a,b)
        c = mvec(gcamvec('^',a,b));
        %if(length(c.gcamdata)==1)
        %   c = blade(c.gcamdata{1});
        %end
      end
      
      function c = mtimes(a,b)
        c = mvec(gcamvec('*',a,b));
        %if(length(c.gcamdata)==1)
        %   c = blade(c.gcamdata{1});
        %end
      end
      
      function c = mrdivide(a,b)
        c = mvec(gcamvec('/',a,b));
      end
      
      function c = not(m)
         c = mvec(gcamvec('~',m));          
      end

      function display(m)
        gcamvec('p',m);
      end
      
      function c = vec(m,vargin)
         if(nargin==1)
             c=gcamvec('v',m);
         else
             c=gcamvec('v',m,vargin);
         end
      end

      function c = maxVal(m)
         indx = 1;
         for i=2:length(m.gcamdata)
            if(m.gcamdata{i}.v > m.gcamdata{indx}.v)
                indx = i;
            end
         end
         c = blade(m.gcamdata{indx});
      end
      
      function c = minVal(m)
         indx = 1;
         for i=2:length(m.gcamdata)
            if(m.gcamdata{i}.v < m.gcamdata{indx}.v)
                indx = i;
            end
         end
         c = blade(m.gcamdata{indx});
      end
      
      function c = uminus(m)
         c = mvec(m.gcamdata);
         for i=1:length(m.gcamdata)
            c.gcamdata{i}.v = -c.gcamdata{i}.v; 
         end          
      end
      
      function c = minus(a,b)
         c = mvec(gcamvec('-',a,b));
      end
      
      function c = plus(a,b)
         c = mvec(gcamvec('+',a,b));
      end

      function c = iszero(a)
         if(isempty(a.gcamdata))
             c = 1;
         else
             c = 0;
         end
      end
      
      function c = grade(a,g)
        c = mvec(gcamvec('g',a,g));
      end
             
      function c = scalar(a)
        c = mvec(gcamvec('g',a,0));
        if(isempty(c.gcamdata))
            c = 0;
        else
            c = c.gcamdata{1}.v;
        end
      end
      
      %function c = maxGrade(m)
      %   indx = 1;
      %   for i=2:length(m.gcamdata)
      %      if(m.gcamdata{i}.v < m.gcamdata{indx}.v)
      %          indx = i;
      %      end
      %   end
      %   c = blade(m.gcamdata{indx});
      %end
      
      
      
   end % methods
end % classdef