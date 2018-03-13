% dv = cw2p(x0,xf,T,w0)
% CW�������Ƶ��������һ��������
% x0:��ʼ���λ���ٶ�
% xf:Ŀ�����λ���ٶ�
% T:ת��ʱ��
% w0:������ٶ�
function [dv1,dv2,dvall] = cw2p(x0,xf,T,w0)
phi = cwstm6(w0,T);
A = phi(1:3,1:3);
B = phi(1:3,4:6);
dv1 = B\(xf(1:3) - A*x0(1:3)) - x0(4:6);
if nargout > 1
    xfb = phi*(x0+[0;0;0;dv1]);
    dv2 = xf(4:6)-xfb(4:6);
end
if nargout > 2
    dvall = sum(abs(dv1)+abs(dv2));
end