% ������Ŀ����ո���(a,e,i,Omega,w,M)����������f�ı仯�ʺͽǼ��ٶ�
function [df,ddf] = dotf(k)
e = k(2);
a = k(1);
p = a*(1-e*e);
f = ma2ta(e,k(6));
r = p/(1+k(2)*cos(f));
df = 1.0/r/r*sqrt(GEarth*p);
if nargout > 1
    ddf = -2*e*sin(f)/r*sqrt(GEarth/p);
end