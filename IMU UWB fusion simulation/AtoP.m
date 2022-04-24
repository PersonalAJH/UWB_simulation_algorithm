function result = AtoP(Acc,Vel)

[m,n] = size(Acc);


pos = zeros(m,n);
dt = 0.01;


for j = 1:n
    for i = 2:m
        pos(i,j) = pos(i-1,j) + Vel(i,j)*dt + Acc(i,j)*(dt^2)*0.5;
    end
end

result = pos;