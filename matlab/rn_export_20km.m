% ���룫�ǶȲ�����ʹ�ü�Ӳ�����������Ե���
disp(strcat('Simulation Start:',datestr(clock)));
clear
close all
load leo20
%% filter
rou_error = 10/1000/3; % ����������
angle_error = 0.05*rad/3; % �ǶȲ������ 

atti_error = 0.05*rad/3;  % ��̬�������
atti_measure_error = 0.005*rad/3; % ��̬�������

realtime = 0;

rou   = zeros(length(t),1);
alpha = zeros(length(t),1);
beta  = zeros(length(t),1);
rou(1) = norm(rt(:,1)) + rou_error*randn;
alpha(1) = atan2(-rt(3,1), rt(1,1)) + angle_error*randn;
beta(1) = asin(rt(2,1)/rou(1)) + angle_error*randn;

x = zeros(6,length(t)); %״̬����
x(:,1) = [rou(1)*cos(beta(1))*cos(alpha(1));rou(1)*sin(beta(1));-rou(1)*cos(beta(1))*sin(alpha(1));0;0;0];
% x(:,1) = [rc(:,1);vc(:,1)];

P = 10^6*eye(6,6);
Q = diag([1e-6*ones(1,3) 1e-6*ones(1,3)]);
R = diag([1 0.1*ones(1,2)]);

err = zeros(length(t),6);
err(1,:) = (x(:,1)-[rc(:,1);vc(:,1)])'*1000;

if realtime == 1
    ax = zeros(6,1);
    fnaverr = figure('Name','�������','Position',[300 400 760 480]);
    ax(1) = subplot(3,2,1); xline =  line('parent',ax(1),'linestyle','-','erase','none','xdata',[],'ydata',[]); grid on;
    ax(3) = subplot(3,2,3); yline =  line('parent',ax(3),'linestyle','-','erase','none','xdata',[],'ydata',[]); grid on;
    ax(5) = subplot(3,2,5); zline =  line('parent',ax(5),'linestyle','-','erase','none','xdata',[],'ydata',[]); grid on;
    ax(2) = subplot(3,2,2); vxline = line('parent',ax(2),'linestyle','-','erase','none','xdata',[],'ydata',[]); grid on;
    ax(4) = subplot(3,2,4); vyline = line('parent',ax(4),'linestyle','-','erase','none','xdata',[],'ydata',[]); grid on;
    ax(6) = subplot(3,2,6); vzline = line('parent',ax(6),'linestyle','-','erase','none','xdata',[],'ydata',[]); grid on;
    mm = [9999*ones(1,3),  9999*ones(1,3); -9999*ones(1,3),  -9999*ones(1,3)];
end 
    
for i=2:length(t)
% ������̬
    phi = atti_error * sin(t(i)/17/2/pi+1);
    theta = atti_error * sin(t(i)/13/2/pi+2);
    psi = atti_error * sin(t(i)/11/2/pi+3);
    Cbo = roty( theta ) * rotx( phi ) * rotz( psi ); % ��̬����
  
% ��Բ�����
    rt_b = Cbo*rt(:,i); % target in chaser's body coordination
    rou(i) = norm(rt_b) + rou_error*randn;
    alpha(i) = atan2(-rt_b(3), rt_b(1)) + angle_error*randn;
    beta(i) = asin(rt_b(2)/norm(rt_b)) + angle_error*randn;

% ����������̬����
  % �����������Ե���
    kcm = kc(i,:);
    kcm(1) = kcm(1) + 0.02/3*randn; %�൱�ڰ볤��������20m
    kcm(3) = kcm(3) + 0.1/3/kcm(1)*randn; % �൱�ڹ������100m������
    kcm(6) = kcm(6) + 0.1/3/kcm(1)*randn; % �൱�ڹ������387m������
 % ����Ŀ��������
    ktm = kt(i,:);
    ktm(1) = ktm(1) + 0.05 + 20/86400*t(i); %�൱�ڰ볤����� = 50m + 20m/��
    ktm(3) = ktm(3) + (0.1+0.1/86400*t(i))/ktm(1); % �൱�ڹ��������� = 100m + 100m/��
    ktm(6) = ktm(6) + (0.2+1/86400*t(i))/ktm(1); % �൱�ڹ��������� = 200m + 1000m/��
    wt = sqrt(GEarth/ktm(1)^3);  %Ŀ�������ٶ�
    rtarget = kt(i,1)*(1-ktm(2)^2)/(1-ktm(2)*cos(kepleru(ktm)-ktm(5)));
    
    Coci = getcoi(kcm);
    Coti = getcoi(ktm);
    
    Cbo_m = roty( theta+atti_measure_error*randn ) * ...
            rotx( phi+atti_measure_error*randn ) * ...
            rotz( psi+atti_measure_error*randn ); % ��̬����

    Cbcot = Cbo_m*Coci*Coti'; 
    
%һ��Ԥ��
    [F A] = Approximate2Order(x(:,i-1),wt,rtarget);
%     [F A] = CWFunction(x(:,i-1),wt);
    xk = x(:,i-1) + F*h + h^2/2*A*F;
    PHI = eye(6,6) + A*h;
    P = PHI*P*PHI' + Q;

% �۲����
    H = [eye(3,3),zeros(3,3)];
% �۲�Э���������
    M = -Cbcot*[cos(beta(i))*cos(alpha(i)),   -rou(i)*cos(beta(i))*sin(alpha(i)),   -rou(i)*sin(beta(i))*cos(alpha(i));
                       sin(beta(i)),                          0,                         rou(i)*cos(beta(i));
               -cos(beta(i))*sin(alpha(i)),   -rou(i)*cos(beta(i))*cos(alpha(i)),   -rou(i)*sin(beta(i))*sin(alpha(i))];
    RR = M*R*M';
% �������
    K = P*H'*inv(H*P*H'+RR);
% ���Ź���
% ��Ӳ�����
    z = rou(i) * [cos(beta(i))*cos(alpha(i)); sin(beta(i)); -cos(beta(i))*sin(alpha(i)) ];
    z = -Cbcot'*z;
    x(:,i) = xk + K*(z - xk(1:3));
% ����
    P = (eye(6,6) - K*H)*P;
% �������
    err(i,:) = (x(:,i)-[rc(:,i);vc(:,i)])'*1000;
% ʵʱ��ͼ����
    if realtime == 1
        for j = 1:6
            if err(i,j) < mm(1,j) || err(i,j) > mm(2,j)
                if err(i,j) < mm(1,j)
                    mm(1,j) = err(i,j) - abs(err(i,j))*1.05;
                end
                if err(i,j) > mm(2,j)
                    mm(2,j) = err(i,j) + abs(err(i,j))*1.05;
                end
                axis(ax(j),[0 t(end) mm(1,j) mm(2,j)]);
           end
        end
        if i-floor(i/20)*20 == 0
            set(xline, 'xdata',t(1:i),'ydata',err(1:i,1));
            set(yline, 'xdata',t(1:i),'ydata',err(1:i,2));
            set(zline, 'xdata',t(1:i),'ydata',err(1:i,3));
            set(vxline,'xdata',t(1:i),'ydata',err(1:i,4));
            set(vyline,'xdata',t(1:i),'ydata',err(1:i,5));
            set(vzline,'xdata',t(1:i),'ydata',err(1:i,6));
            drawnow
        end
    end
end
%% ��Թ켣
start = 200;
% figure('Name','��Թ켣','Position',[10 10 560 700]);
% subplot 411,plot(rc(1,:),rc(3,:)),grid on,ylabel('z(km)'),hold on,plot(x(1,start:end),x(3,start:end),'r');
% subplot 412,plot(rc(1,:),rc(2,:)),grid on,ylabel('y(km)'),xlabel('x(km)'),hold on,plot(x(1,start:end),x(2,start:end),'r');
% subplot 413,plot(vc(1,:),vc(3,:)),grid on,ylabel('vz'),hold on,plot(x(4,start:end),x(6,start:end),'r');
% subplot 414,plot(vc(1,:),vc(2,:)),grid on,ylabel('vy'),xlabel('vx'),hold on,plot(x(4,start:end),x(5,start:end),'r');

% figure('Name','�������');
% subplot 211,plot(t,rou),grid on,ylabel('\rho');
% subplot 212,plot(t,alpha,t,beta),grid on,ylabel('\alpha \beta');
% �������
if realtime == 1
    figure(fnaverr);
else
    figure('Name','�������','Position',[300 400 760 480]);
    plot6(t(start:end),err(start:end,:),'label','x(m)','y(m)','z(m)','vx(m/s)','vy(m/s)','vz(m/s)');
end
relnav_error = [mean(err(start:end,1)), mean(err(start:end,2)), mean(err(start:end,3)),...
                mean(err(start:end,4)), mean(err(start:end,5)), mean(err(start:end,6));...
                std(err(start:end,1))*3, std(err(start:end,2))*3, std(err(start:end,3))*3, ...
                std(err(start:end,4))*3, std(err(start:end,5))*3, std(err(start:end,6))*3];
disp(relnav_error);
disp('������');disp([rou_error*3*1000,angle_error*3*deg]);
disp('������');disp([norm(relnav_error(2,1:3)/3)*3 , norm(relnav_error(2,4:6)/3)*3 ]);
