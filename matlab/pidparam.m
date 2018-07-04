% �������ϵͳ��PID���Ʋ���
% [kp ki kd] = pidparam(I,wn,ksi,tao)
% I��ת������
% wn: ϵͳ����
% ksi: ϵͳ����
% tao������ʱ�䳣��,������������Ϊ10/(ksi*wn)
function [kp,ki,kd] = pidparam(I,wn,ksi,tao)
if nargin == 0  %test
    I = 2.33;%1.98;%1.81;%
    ksi = 0.707;%ksia(i);
    wn = 0.07;
    TT = 10/(ksi*wn);
else
    if nargin == 3
        TT = 10/(ksi*wn);
    else
        TT = tao;
    end
    kp = I*(wn^2 + 2*ksi*wn/TT);
    kd = I*(2*ksi*wn+1/TT);
    ki = I*wn^2/TT;
end