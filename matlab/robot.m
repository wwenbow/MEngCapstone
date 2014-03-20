function xdot= robot(t,x)  
g= 9.8 ; L= 1 ; m= 10 ; u= sin(2*pi*t);
xdot= [x(2) ; -g*L*sin(x(1)) - L*x(2)^2 + u/m] ;
end
