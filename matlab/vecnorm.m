% ��ʸ����ģ
function r = vecnorm(p)
[m n] = size(p);
if m==3
    i = 1;
elseif n==3
    i = 2;
else
    error('����p���л�����Ҫ��һ��Ϊ3');
end
r = sqrt(sum(p(:,1:3).^2,i));