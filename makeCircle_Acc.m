
NumW = 40000;        %원을 짜르는 개수
Acc_W = 625000;     %원 반지름 크기 6.25m (Acc_W*dt) 최고 속도 0.9817m/s  최고 가속도 0.0016m/s^2  직선 acc = 0.001 m/s^2

dt = 0.01;          % 10ms dt
std = 0.002;            % xsens 의 noise 는 0.002 m/s^2 


for i = 1:NumW
    
    acc_Circle(i,1) = -Acc_W*(2*pi/NumW)^2*cos(i*pi*2/NumW);
    acc_Circle(i,2) = -Acc_W*(2*pi/NumW)^2*sin(i*pi*2/NumW);
    
end

vel_Circle = zeros(NumW,2);
vel_Circle(1,1) = acc_Circle(1,1)*dt;%-0.0013;
vel_Circle(1,2) = (acc_Circle(1,2)+Acc_W*(2*pi/NumW))*dt;


for j = 1:2
    for i = 2:NumW
        vel_Circle(i,j) = vel_Circle(i-1,j) + acc_Circle(i,j)*dt;
    end
end




Circle_acc = zeros(NumW,2);

for j = 1:2
    for i = 2:NumW
        Circle_acc(i,j) = Circle_acc(i-1,j) + vel_Circle(i,j)*dt + acc_Circle(i,j)*(dt^2)*0.5;
    end
end

% Circle_acc = Circle_acc/dt;

Circle = zeros(NumW,2);

for i = 1:NumW
    Circle(i,1) = Acc_W*cos(i*pi*2/NumW)*dt*dt;
    Circle(i,2) = Acc_W*sin(i*pi*2/NumW)*dt*dt;
end

vel_Circle_2 = zeros(NumW,2);

for i = 1:NumW

    vel_Circle_2(i,1) = -Acc_W*(2*pi/NumW)*sin(i*pi*2/NumW);
    vel_Circle_2(i,2) = Acc_W*(2*pi/NumW)*cos(i*pi*2/NumW);
    
end


vel_Circle_mea2 = zeros(NumW,2);
en_std = 0.5;

for i = 1:NumW

    vel_Circle_mea2(i,1) = -Acc_W*(2*pi/NumW)*sin(i*pi*2/NumW)+ normrnd(0,en_std);
    vel_Circle_mea2(i,2) = Acc_W*(2*pi/NumW)*cos(i*pi*2/NumW)+ normrnd(0,en_std);
    
end


Circle(:,1) = Circle(:,1)-Acc_W*dt*dt;

% plot(Circle(:,1),Circle(:,2))
% hold on
% plot(Circle_acc(:,1),Circle_acc(:,2))


    
    