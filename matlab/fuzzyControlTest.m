function xdot = fuzzyPendulumnModel(t,x)
% Alogrithm based on "Stability Analysis and Performance Deign for 
% Fuzzy-Model-Based Control System under Imperfect Premise Matching" by H.K.
% Lam

%==============================================================================
% Dynamic model of inverted pendulumn
%==============================================================================
% Variables: 
% x1(t) : angular displacememnt (rad)
% x2(t) : angular velocity (rad/s)
% x3(t) : displacement (meter)
% x4(t) : velocity (meter/second)
% u(t)  : force applied to cart (N)

% Constants:
g = 9.8;           % g  : acceleration due to gravity 9.8 m/s^2 
m = 0.022;         % m  : mass of pendulumn (kg)
M = 1.3282;        % M  : mass of cart (kg)
l = 0.304;         % l  : length center of mass of pendulumn to shaft axis (m)
J0 = (m.*l.^2)./3; % J0 : moment of inertia of perndulum around center of mass 
                   %      (kgm^2)
F0 = 22.915;       % F0 : friction factor of cart (N/(m*s))
F1 = 0.007056;     % F1 : friction factor of pendulumn (N/(rad*s))

%Basic Equations
%{
dx1 = x2;
dx2 = ( -F1.*(M+m).*x2 - m.^2*l.^2*x2.^2*sin(x1).*cos(x1) + F0.*m.*l.*x4 + ...
        (M+m).*m.*g.*l.*sin(x1) - m.*l.*cos(x1).u ) ...
        ./ ( (M+m).*(J0+.*l.^2) - (m.^2).*(l.^2).*(cos(x1)).^2);
dx3 = x4;
dx4 = ( F1.*m.*l.*x2.*cos(x1) + (J+m.*l.^2).*m.*l.(x2.^2).*sin(x1) ...
        - F0.*(J0 + m.*l.^2).*x4 - (m.^2).*g.*(l.^2).*sin(x1).*cos(x1) ...
        + (J0 + m.*l.^2).*u) ...
        ./ ( (M+m).*(J0+m.*l.^2) - (m.^2).*(l.^2).*(cos(x1)).^2 );
%}

% State Space Equations
% dx(t) = sum(1 to 2){ w_i * ( A_i*x(t) + B_i*u(t) ) }

a1 = (M+m)*(J0+m*l^2) - m^2*l^2;
a2 = (M+m)*(J0+m*l^2) - m^2*l^2*cos(pi/3)^2;

A1 = [ 0 1 0 0;
      (M+m)*m*g*l/a1 -F1*(M+m)/a1 0 F0*m*l/a1 ;
       0 0 0 1;
      -(m^2)*g*(l^2)/a1 F1*m*l/a1 0 -F0*(J0+m*l^2)/a1 ];
A2 = [ 0 1 0 0;
      3*sqrt(3)/(2*pi)*(M+m)*m*g*l/a2 -F1*(M+m)/a2 0 F0*m*l*cos(pi/3)/a2;
       0 0 0 1;
      -3*sqrt(3)/(2*pi)*m^2*g*l^2*cos(pi/3)/a2 F1*m*l*cos(pi/3)/a2 0 ...
      -F0*(J0+m*l^2)/a1];

B1 = [ 0; -m*l/a1; 0 ;(J0+m*l^2)/a1 ];
B2 = [ 0; -m*l*cos(pi/3)/a2; 0; (J0+m*l^2)/a2];

%w1 ,w2 = ???
% model = ss(w1*A1 + w2*A2, w1*B1 + w2*B2, [], []);
w1 = (1-[1./(1 + e.^(-7.*(x(1) - pi/6)))]).*(1./(1 + e.^(-7.*(x(1)+pi/6))));
w2 = 1 - w1;

u = sin(2*pi*t);

xdot = w1.*A1*x + w2.*A2*x + w1.*B1*u + w2.*B2*u;
