% ���������
%   precessioin(year,month,day,hour,minute,second)����UTC����ʱʱ�̵�������
%   precessioin(mjd)�����Լ������mjdʱ�̵�������
%
%   ����һ��ʸ����J2000.0��������ϵ�µ�����Ϊr������˲ʱƽ�������ϵ�е�����Ϊ precession*r
%   ��֮��һ��ʸ����˲ʱƽ�������ϵ�µ�����Ϊr������J2000.0��������ϵ�е�����Ϊ precession'*r
%   ��������u1,u2,u3��ת��ΪPrecession = Rz(-Z)*Ry(theta)*Rz(-zeta)
% 
% Example:
%      precession(2006,1,1,0,0,0);
%      precession(53736)
%
% See also precession_angle, nutation_angle, nutation
function PR = precession(Y,M,D,h,m,s)
if nargin == 6
    TDT = utc2tdt(date2mjd(Y,M,D,h,m,s));
elseif nargin == 1
    TDT = utc2tdt(Y);
else
    error('���������Ŀ����');
end
[zeta,theta,Z] = precession_angle(TDT);
PR = rotz(-Z)*roty(theta)*rotz(-zeta);