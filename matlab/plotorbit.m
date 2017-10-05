%  �Թ������elemΪ��ֵ�����ն����˶�����һ��������ڵ���ά���
function plotorbit(elem)
Color=rand(1,3);
while all(Color>0.4)
    Color=rand(1,3);
end    
rv = zeros(181,6);
for M = 0:1:180
    elem(6) = M*RAD;
    rv(M+1,1:6) = kepler2cart(elem);
end
plot3(rv(:,1),rv(:,2),rv(:,3),'color',Color),grid on,hold on;