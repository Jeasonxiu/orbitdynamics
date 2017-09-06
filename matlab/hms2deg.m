% ��ʱ�����ʾ�ĽǶ�ת��Ϊ��
% d = hms2deg(h,m,s)
% example:
%     d = hms2deg(12,15,19)
%     d = hms2deg(-12,23,55)
%     d = hms2deg([12,15,19]);
%     d = hms2deg([-12,15,19]);
function d = hms2deg(h,m,s)
if nargin == 3
    if h>24 || h<0 || m>60 || m<0 || s>60 || s<0
        error('dfm2deg:�����СʱӦ��0~24֮�ڣ��֡���Ӧ��0~50֮��');
    end
    d = (h + m/60 + s/3600)*15;
elseif nargin == 1
    if h(1)>24 || h(1)<0 || h(2)>60 || h(2)<0 || h(3)>60 || h(3)<0
        error('dfm2deg:�����СʱӦ��0~24֮�ڣ��֡���Ӧ��0~50֮��');
    end
    d = (h(1) + h(2)/60 + h(3)/3600)*15;
else
    error('hms2deg:���������������');
end