function m = rotor(x,radians)

    if(~exist('radians','var'))
        radians = true;
    end

    if(isnumeric(x) && length(x)==3)
        if(~radians)
            x = x/180*pi;
        end

        e1 = mvec([1 0 0]);
        e2 = mvec([0 1 0]);
        e3 = mvec([0 0 1]);

        B = e2*e3;
        Rx = cos(x(1)/2)-B*sin(x(1)/2);

        B = e1*e3;
        Ry = cos(x(2)/2)-B*sin(x(2)/2);

        B = e1*e2;
        Rz = cos(x(3)/2)-B*sin(x(3)/2);

        m = Rx*Ry*Rz;
    end
end