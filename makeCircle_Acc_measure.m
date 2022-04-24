
NumW = 40000;        %원을 짜르는 개수
Acc_W = 625000;     %원 반지름 크기 6.25m (Acc_W*dt) 최고 속도 0.9817m/s  최고 가속도 0.0016m/s^2  직선 acc = 0.001 m/s^2

dt = 0.01;          % 10ms dt
std = 0.002;            % xsens 의 noise 는 0.002 m/s^2 


for i = 1:NumW
    
    acc_Circle_mea(i,1) = -Acc_W*(2*pi/NumW)^2*cos(i*pi*2/NumW)+normrnd(0,std);
    acc_Circle_mea(i,2) = -Acc_W*(2*pi/NumW)^2*sin(i*pi*2/NumW)+normrnd(0,std);
    
end

vel_Circle_mea = zeros(NumW,2);
vel_Circle_mea(1,1) = acc_Circle_mea(1,1)*dt;%-0.0013;
vel_Circle_mea(1,2) = (acc_Circle(1,2)+Acc_W*(2*pi/NumW))*dt;


for j = 1:2
    for i = 2:NumW
        vel_Circle_mea(i,j) = vel_Circle_mea(i-1,j) + acc_Circle_mea(i,j)*dt;
    end
end




Circle_acc_mea = zeros(NumW,2);

for j = 1:2
    for i = 2:NumW
        Circle_acc_mea(i,j) = Circle_acc_mea(i-1,j) + vel_Circle_mea(i,j)*dt + acc_Circle_mea(i,j)*(dt^2)*0.5;
    end
end

% Circle_acc = Circle_acc/dt;

plot(Circle_acc_mea(:,1),Circle_acc_mea(:,2))



%% Linear 속도볼때 사용
% total_vel_Circle= zeros(NumW,1);
% for i = 1:NumW
%     
%     total_vel_Circle(i,1) = sqrt(vel_Circle(i,1)^2 + vel_Circle(i,2)^2);
%     
% end

    
    