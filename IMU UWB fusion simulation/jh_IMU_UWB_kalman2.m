function result = jh_IMU_UWB_kalman2(input, measure, measure2)

[datasize,dummy1] = size(input);

state_estimate = zeros(4,datasize);
state_estimate_hat = zeros(4,datasize);
estimate_pos = zeros(2,datasize);



dt = 0.01;

A = [1 dt 0 0;0 1 0 0;0 0 1 dt; 0 0 0 1];
B = [(dt^2)*0.5 0;dt 0;0 (dt^2)*0.5;0 dt];
C = [1 0 0 0;0 1 0 0;0 0 1 0;0 0 0 1];

% Q = [dt^4/4 dt^3/2 0 0;dt^3/2 dt^2 0 0;0 0 dt^4/4 dt^3/2;0 0 dt^3/2 dt^2];  % Q = sigma.^2 * B*transpose(B)       sigma 는 공정오차
Q2 = eye(4)*0.01;
P = eye(4);
R = [0.15 0 0 0;0 0 0 0;0 0 0.15 0; 0 0 0 0];
% R = eye(2)*0.15;


for i = 2:datasize-1
    
    input_data = transpose(input(i,:));
%     measure_data = transpose(measure(i,:));
    measure_data = [measure(i,1);measure2(i,1);measure(i,2);measure2(i,2)];
    
    
    state_estimate_hat(:,i) = A*state_estimate(:,i-1) + B*input_data;
    P_hat = A*P*transpose(A) + Q2;
    
    
    K = P_hat * transpose(C) / (C*P_hat*transpose(C) + R);
    state_estimate(:,i) = state_estimate_hat(:,i) + K*(measure_data - C*state_estimate_hat(:,i));
    P = (eye(4)-K*C)*P_hat;
    
   estimate_pos(:,i) = state_estimate([1,3],i);
   
end

result = transpose(estimate_pos);






