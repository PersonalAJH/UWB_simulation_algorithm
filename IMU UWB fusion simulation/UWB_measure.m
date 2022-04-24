function result = UWB_measure(A)

[m,n] = size(A);
A_dot = zeros(m,n);

for i = 1:m
    for j = 1:n
    A_dot(i,j) = A(i,j) + normrnd(0,0.15); %1sigma = 68%, 2sigma = 95% (30cm 안에 들어오는 값이 95%)
    end
end

result = A_dot;




