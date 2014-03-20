x0= [5*pi/12 0 0 0 ] ; % define time interval [t0 tf] 
tint= [0 1]' ; % initial conditions 
[t,x]= ode23('fuzzyPendulumnTest', tint, x0); 
plot(t,x) 
