% velocity �� Į�������� ���������� �������� �׽�Ʈ


makeCircle_Acc                % ���ϴ� �׸��� acc, vel �� �����

makeCircle_Acc_measure        % acc�� normpdf �߰�( IMU ������ �������� �߰�)

% Circle = AtoP(acc_Circle,vel_Circle);     %acc �� position �����


% jh = AtoP(acc_Circle_mea,vel_Circle_mea2);      %���ڴ� ����
Acc_mea_Circle = AtoP(acc_Circle_mea,vel_Circle_mea);     % IMU ������ ������ �����͸� �̿��ؼ� position �����
UWB_mea_Circle = UWB_measure(Circle);       %������ ���� pos ���� normpdf �߰�( UWB������ ��������)


% test1 = jh_IMU_UWB_kalman(acc_Circle_mea,UWB_mea_Circle);    %�Ϲ����� Kalman filter�� ��ġ����

% test2= algorithm2(acc_Circle_mea,vel_Circle_mea,UWB_mea_Circle,100);  %IMU �������� �ӵ��� �����Ѵٰ� ����

test3= algorithm2(acc_Circle_mea,vel_Circle_2,UWB_mea_Circle,100);  %���ڴ����� velocity �� ��´ٰ� ����
test4 = jh_IMU_UWB_kalman2(acc_Circle_mea,UWB_mea_Circle,vel_Circle_2);     %Kalman filter�� encoder�� �����͸� �޾Ƽ� ���
test5 = algorithm5(acc_Circle_mea,UWB_mea_Circle,100);

plot(Acc_mea_Circle(:,1),Acc_mea_Circle(:,2),'o')   %IMU

hold on

plot(UWB_mea_Circle(:,1),UWB_mea_Circle(:,2),'o')   %UWB
plot(test4(:,1),test4(:,2),'o') %Kalman filter
% plot(test3(:,1),test3(:,2),'o') %���ڴ����� �ӵ��� ��´ٰ� ����
plot(test5(:,1),test5(:,2),'o')
plot(Circle(:,1),Circle(:,2))   %standard

legend({'Only IMU','Only UWB','Kalman filter','Proposed Algorithm','Reference Path'},'FontSize',14)


xlabel("x distance(m)",'Fontsize',30)
ylabel("y distance(m)",'Fontsize',30)
xlim([-140 10])
ylim([-70 70])

%%

[datasize,dummy] = size(Circle);
e_kalman = zeros(datasize,dummy);           e_kalman2 = zeros(datasize,1);
e_NewAlgorhtm = zeros(datasize,dummy);      e_NewAlgorhtm2 = zeros(datasize,1);
e_IMU = zeros(datasize,dummy);              e_IMU2 = zeros(datasize,1);
e_UWB = zeros(datasize,dummy);              e_UWB2 = zeros(datasize,1);


for i = 1:datasize
    
    e_kalman(i,1) = Circle(i,1)-test4(i,1);
    e_kalman(i,2) = Circle(i,2)-test4(i,2);
    
    e_NewAlgorhtm(i,1) = Circle(i,1)-test3(i,1);
    e_NewAlgorhtm(i,2) = Circle(i,2)-test3(i,2);
    
    
    e_UWB(i,1) = Circle(i,1)-UWB_mea_Circle(i,1);
    e_UWB(i,2) = Circle(i,2)-UWB_mea_Circle(i,2);
    
    
    e_IMU(i,1) = Circle(i,1)-Acc_mea_Circle(i,1);
    e_IMU(i,2) = Circle(i,2)-Acc_mea_Circle(i,2);
    

    e_kalman2(i,1) = norm(e_kalman(i,:));
    e_NewAlgorhtm2(i,1) = norm(e_NewAlgorhtm(i,:));
    e_IMU2 = norm(e_IMU(i,:));
    e_UWB2 = norm(e_UWB(i,:));

end

m1 = mean(e_kalman2);
m2 = mean(e_NewAlgorhtm2);
m3 = mean(e_IMU2);
m4 = mean(e_UWB2);
