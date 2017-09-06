% ̫��ͬ���ع�������
function f = SunSyncRepeating(x)
a = x(1);
i = x(2);
R = 44;
N = 3;
n = sqrt(GEarth/a^3);
TN = 2*pi/n*(1-1.5*J2*(Re/a)^2*(3-2.5*sin(i)*sin(i)));
dotOmega = -1.5*n*J2*(Re/a)^2*cos(i);
% �ع�����Ҫ�󣬷��̵�ֵΪ����
f(1) = R*TN*(We + dotOmega) - N*2*pi;
% ̫��ͬ�������Ҫ�󣬹����ÿ�����0.9856�ȣ����̵�ֵΪ����
f(2) = dotOmega*86164.09 - 0.9856*rad; 
% f = f*deg*100;