function recvCallback(com,event)
    N=50;
    persistent temp accX accY accZ gyrX gyrY gyrZ;
    if isempty(temp)
        temp=nan(1,N);
        accX=nan(1,N);
        accY=nan(1,N);
        accZ=nan(1,N);
        gyrX=nan(1,N);
        gyrY=nan(1,N);
        gyrZ=nan(1,N);
    end
    if strcmp(event.Type,'BytesAvailable')
        data=fscanf(com,'|%x,%x,%x,%x,%x,%x,%x$');
        data=double(typecast(uint16(data),'int16'));
        data=data.*[1/256 16/32767 16/32767 16/32767 2000/32767 2000/32767 2000/32767]';
        data=data+[25 0 0 0 0 0 0]';
        temp=[data(1) temp(1:end-1)];
        accX=[data(2) accX(1:end-1)];
        accY=[data(3) accY(1:end-1)];
        accZ=[data(4) accZ(1:end-1)];
        gyrX=[data(5) gyrX(1:end-1)];
        gyrY=[data(6) gyrY(1:end-1)];
        gyrZ=[data(7) gyrZ(1:end-1)];
        figure(1);
        subplot(311);
        plot(1:N,temp);
        axis([1 N 0 40]);
        ylabel('T [\circC]');
        subplot(312);
        plot(1:N,accX,1:N,accY,1:N,accZ);
        axis([1 N -3 3]);
        ylabel('a [g]');
        subplot(313);
        plot(1:N,gyrX,1:N,gyrY,1:N,gyrZ);
        axis([1 N -500 500]);
        ylabel('\omega [\circ/s]');
    end
