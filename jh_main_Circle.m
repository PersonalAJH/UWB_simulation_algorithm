% velocity 를 칼만필터의 추정값으로 가져오는 테스트


makeCircle_Acc                % 원하는 그림의 acc, vel 를 만들기

makeCircle_Acc_measure        % acc에 normpdf 추가( IMU 센서의 측정오차 추가)

% Circle = AtoP(acc_Circle,vel_Circle);     %acc 로 position 만들기


% jh = AtoP(acc_Circle_mea,vel_Circle_mea2);      %엔코더 측정
Acc_mea_Circle = AtoP(acc_Circle_mea,vel_Circle_mea);     % IMU 센서로 측정한 데이터를 이용해서 position 만들기
UWB_mea_Circle = UWB_measure(Circle);       %오차가 없는 pos 에서 normpdf 추가( UWB센서의 측정오차)


% test1 = jh_IMU_UWB_kalman(acc_Circle_mea,UWB_mea_Circle);    %일반적인 Kalman filter로 위치추정

% test2= algorithm2(acc_Circle_mea,vel_Circle_mea,UWB_mea_Circle,100);  %IMU 센서에서 속도를 추정한다고 가정

test3= algorithm2(acc_Circle_mea,vel_Circle_2,UWB_mea_Circle,100);  %엔코더에서 velocity 를 얻는다고 가정
test4 = jh_IMU_UWB_kalman2(acc_Circle_mea,UWB_mea_Circle,vel_Circle_2);     %Kalman filter에 encoder의 데이터를 받아서 사용
test5 = algorithm5(acc_Circle_mea,UWB_mea_Circle,100);

plot(Acc_mea_Circle(:,1),Acc_mea_Circle(:,2),'o')   %IMU

hold on

plot(UWB_mea_Circle(:,1),UWB_mea_Circle(:,2),'o')   %UWB
plot(test4(:,1),test4(:,2),'o') %Kalman filter
% plot(test3(:,1),test3(:,2),'o') %엔코더에서 속도를 얻는다고 가정
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
