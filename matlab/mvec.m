classdef mvec < handle
   properties (Hidden)

   end
   % The following properties can be set only by class methods
   properties (SetAccess = private)
      gcamdata=[];
   end
   methods
      function m = mvec(x)
         if(ischar(x))
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
      
      function c = not(m)
         c = mvec(gcamvec('~',m));          
      end

      function display(m)
        gcamvec('p',m);
      end

      function c = vec(m,dim)
         c=gcamvec('v',m,dim);
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