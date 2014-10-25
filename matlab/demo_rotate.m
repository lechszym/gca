function h = demo_rotate(m,n,h,m2,n2)

if(~exist('m','var'))
    m = [1 0 0];
end

if(~exist('n','var'))
    n = m;
    n(2) = 0.05;
    n = n/norm(n);
end

if(~exist('m2','var'))
    m2 = [];
end

if(~exist('n2','var'))
    n2 = [];
end


if(~isempty(m) && ~isempty(n))   
    m = mvec(m);
    n = mvec(n);

    R = m*n;
    Rc = ~R;
end

if(~isempty(m2) && ~isempty(n2))   
    m2 = mvec(m2);
    n2 = mvec(n2);
    R2 = m2*n2;
    R2c = ~R2;
end

if(~exist('h','var') || isempty(h))
    h = plotobj(3);
    daspect([1 1 1]);
    grid on;
    drawnow();
    pause(0.1);
end

if(isempty(m) || isempty(n)) 
    return;
end
    
data(:,:,1) = get(h,'xdata');
data(:,:,2) = get(h,'ydata');
data(:,:,3) = get(h,'zdata');

data = shiftdim(data,2);
[D,N1,N2] = size(data);
data = reshape(data,3,N1*N2);
N = size(data,2);

a = cell(N,1);
for i=1:N
    a{i} = mvec(data(:,i));
end

for f=1:200
    if(~isempty(m2) && ~isempty(n2))   
        R2 = R*R2*Rc;
        R2c = ~R2;
    end
    for i=1:N
       a{i} = R*a{i}*Rc;
       if(~isempty(m2) && ~isempty(n2))   
          a{i} = R2*a{i}*R2c; 
       end
       data(:,i) = a{i}.vec(3);
    end
    data = reshape(data,3,N1,N2);
    data = shiftdim(data,1);
    set(h,'xdata',data(:,:,1),'ydata',data(:,:,2),'zdata',data(:,:,3));
    drawnow();
    pause(0.01);
    data = shiftdim(data,2);
    data = reshape(data,3,N1*N2);
    
end



end


function h = plotobj(num)

    if(~exist('num','var'))
        num = 1;
    end
    
    if(num == 1)
        r=1; u=linspace(0,2*pi,60); v=linspace(0,2*pi,60); [u,v]=meshgrid(u,v);
        x=(r+cos(u/2).*sin(v)-sin(u/2).*sin(2*v)).*cos(u); y=(r+cos(u/2).*sin(v)-sin(u/2).*sin(2*v)).*sin(u); z=sin(u/2).*sin(v)+cos(u/2).*sin(2*v);
        h = surface(x,y,z,'FaceColor','interp','EdgeColor','none','FaceLighting','phong');
        camlight left
        colormap(hot)
        view(3)
    elseif(num == 2)
        vertex_matrix = [0 0 0; 1 0 0; 1 1 0; 0 1 0; 0 0 1; 1 0 1; 1 1 1; 0 1 1];
        faces_matrix = [1 2 6 5; 2 3 7 6; 3 4 8 7; 4 1 5 8; 1 2 3 4; 5 6 7 8];
        h = patch('Vertices',vertex_matrix,'Faces',faces_matrix,...
        'FaceVertexCData',hsv(6),'FaceColor','flat');
        xlim([-2 2]);
        ylim([-2 2]);
        zlim([-2 2]);
        view(3)
    else
        vertex_matrix = [0 0 0; 1 0 0; 0 0 .25; 1 0 .25;...
                         0 1 0; 1 1 0; 0 1 1; 1 1 1;...
                         0 7 0; 1 7 0; 0 7 1; 1 7 1;...
                         0 7.5 0; 1 7.5 0; 0 7.5 1.5; 1 7.5 1.5;...
                         1 3 0.4; 7 3 0.4; 7 3 0.6; 1 3 0.6;...
                         1 4.5 0.4; 7 4 0.4; 7 4 0.6; 1 4.5 0.6;...
                         0 3 0.4; -6 3 0.4; -6 3 0.6; 0 3 0.6;...
                         0 4.5 0.4; -6 4 0.4;-6 4 0.6; 0 4.5 0.6;...
                         ];
        vertex_matrix(:,1) = vertex_matrix(:,1)-0.5; 

            
        faces_matrix = [1 3 4 2; 1 5 6 2; 1 3 7 5; 2 4 8 6; 3 7 8 4;
                        5 7 11 9; 7 11 12 8; 8 12 10 6; 5 9 10 6;...
                        9 11 15 13; 11 15 16 12; 12 16 14 10; 9 13 14 10; 13 15 16 14;...
                        17 18 19 20; 18 19 23 22;21 22 23 24;20 19 23 24; 17 18 22 21;... 
                        25 26 27 28; 26 27 31 30;29 30 31 32;28 27 31 32; 25 26 30 29];
        h = patch('Vertices',vertex_matrix,'Faces',faces_matrix,...
        'FaceVertexCData',hsv(size(faces_matrix,1)),'FaceColor','flat');
        daspect([1 1 1]);

        view(3)
        xlim([-9 9]);
        ylim([-9 9]);
        zlim([-9 9]);
        view(3)
        
        
        
    end
end

