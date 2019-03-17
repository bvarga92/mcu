clear all;
clc;

%% parameterek
W=52; % ablakszelesseg a szegmentalashoz
files={ % adatfajlok (szintaxis: {'fajlnev', osztaly})
    {'lsm6dsox_2019_03_17_22_45_33.txt',0}
    {'lsm6dsox_2019_03_17_22_46_47.txt',1}
};

%% feature vektor osszeallitasa
% szintaxis: {'forras_feature', kuszob}
% forras lehet: accX, accY, accZ, acc, acc2, gyrX, gyrY, gyrZ, gyr, gyr2
% feature lehet: mean, var, energy, pk2pk, cross, crossP, crossN, peak, peakP, peakN, min, max
% kuszob csak a kovetkezokhoz kell: cross, crossP, crossN, peak, peakP, peakN
F={
    {'accX_mean'}
    {'accX_var'}
    {'accX_energy'}
    {'accX_pk2pk'}
    {'accX_cross',0}
    {'accX_crossP',0}
    {'accX_crossN',0}
    {'accX_peaks',0.5}
    {'accX_peaksP',0.5}
    {'accX_peaksN',0.5}
    {'accX_min'}
    {'accX_max'}
    {'accY_mean'}
    {'accY_var'}
    {'accY_energy'}
    {'accY_pk2pk'}
    {'accY_cross',0}
    {'accY_crossP',0}
    {'accY_crossN',0}
    {'accY_peaks',0.5}
    {'accY_peaksP',0.5}
    {'accY_peaksN',0.5}
    {'accY_min'}
    {'accY_max'}
    {'accZ_mean'}
    {'accZ_var'}
    {'accZ_energy'}
    {'accZ_pk2pk'}
    {'accZ_cross',0}
    {'accZ_crossP',0}
    {'accZ_crossN',0}
    {'accZ_peaks',0.5}
    {'accZ_peaksP',0.5}
    {'accZ_peaksN',0.5}
    {'accZ_min'}
    {'accZ_max'}
%     {'acc_mean'}
%     {'acc_var'}
%     {'acc_energy'}
%     {'acc_pk2pk'}
%     {'acc_cross',0}
%     {'acc_crossP',0}
%     {'acc_crossN',0}
%     {'acc_peaks',0.5}
%     {'acc_peaksP',0.5}
%     {'acc_peaksN',0.5}
%     {'acc_min'}
%     {'acc_max'}
%     {'acc2_mean'}
%     {'acc2_var'}
%     {'acc2_energy'}
%     {'acc2_pk2pk'}
%     {'acc2_cross',0}
%     {'acc2_crossP',0}
%     {'acc2_crossN',0}
%     {'acc2_peaks',0.25}
%     {'acc2_peaksP',0.25}
%     {'acc2_peaksN',0.25}
%     {'acc2_min'}
%     {'acc2_max'}
    {'gyrX_mean'}
    {'gyrX_var'}
    {'gyrX_energy'}
    {'gyrX_pk2pk'}
    {'gyrX_cross',0}
    {'gyrX_crossP',0}
    {'gyrX_crossN',0}
    {'gyrX_peaks',100}
    {'gyrX_peaksP',100}
    {'gyrX_peaksN',100}
    {'gyrX_min'}
    {'gyrX_max'}
    {'gyrY_mean'}
    {'gyrY_var'}
    {'gyrY_energy'}
    {'gyrY_pk2pk'}
    {'gyrY_cross',0}
    {'gyrY_crossP',0}
    {'gyrY_crossN',0}
    {'gyrY_peaks',100}
    {'gyrY_peaksP',100}
    {'gyrY_peaksN',100}
    {'gyrY_min'}
    {'gyrY_max'}
    {'gyrZ_mean'}
    {'gyrZ_var'}
    {'gyrZ_energy'}
    {'gyrZ_pk2pk'}
    {'gyrZ_cross',0}
    {'gyrZ_crossP',0}
    {'gyrZ_crossN',0}
    {'gyrZ_peaks',100}
    {'gyrZ_peaksP',100}
    {'gyrZ_peaksN',100}
    {'gyrZ_min'}
    {'gyrZ_max'}
%     {'gyr_mean'}
%     {'gyr_var'}
%     {'gyr_energy'}
%     {'gyr_pk2pk'}
%     {'gyr_cross',0}
%     {'gyr_crossP',0}
%     {'gyr_crossN',0}
%     {'gyr_peaks',100}
%     {'gyr_peaksP',100}
%     {'gyr_peaksN',100}
%     {'gyr_min'}
%     {'gyr_max'}
%     {'gyr2_mean'}
%     {'gyr2_var'}
%     {'gyr2_energy'}
%     {'gyr2_pk2pk'}
%     {'gyr2_cross',0}
%     {'gyr2_crossP',0}
%     {'gyr2_crossN',0}
%     {'gyr2_peaks',10000}
%     {'gyr2_peaksP',10000}
%     {'gyr2_peaksN',10000}
%     {'gyr2_min'}
%     {'gyr2_max'}
};

%% beolvasas es feature szamitas
features=[];
for ff=1:length(files)
    fp=fopen(files{ff}{1});
    data=cell2mat(textscan(fp,'%f %f %f %f %f %f','HeaderLines',1));
    fclose(fp);
    data(:,1:3)=data(:,1:3)/1000;
    features_ff=zeros(floor(size(data,1)/W),length(F));
    
    for ii=1:length(F)
        sep=find((F{ii}{1}=='_'));
        source=F{ii}{1}(1:sep-1);
        feature=F{ii}{1}(sep+1:end);
        switch source
            case 'accX'
                input=data(:,1);
            case 'accY'
                input=data(:,2);
            case 'accZ'
                input=data(:,3);
            case 'acc'
                input=sqrt(data(:,1).^2+data(:,2).^2+data(:,3).^2);
            case 'acc2'
                input=data(:,1).^2+data(:,2).^2+data(:,3).^2;
            case 'gyrX'
                input=data(:,4);
            case 'gyrY'
                input=data(:,5);
            case 'gyrZ'
                input=data(:,6);
            case 'gyr'
                input=sqrt(data(:,4).^2+data(:,5).^2+data(:,6).^2);
            case 'gyr2'
                input=data(:,4).^2+data(:,5).^2+data(:,6).^2;
            otherwise
                error('Invalid source: %s',source);
        end
        switch feature
            case 'mean' % atlag
                for jj=1:size(features_ff,1)
                    features_ff(jj,ii)=mean(input((jj-1)*W+1:jj*W));
                end
            case 'var' % variancia
                for jj=1:size(features_ff,1)
                    features_ff(jj,ii)=var(input((jj-1)*W+1:jj*W),1);
                end
            case 'energy' % energia
                for jj=1:size(features_ff,1)
                    features_ff(jj,ii)=sum(input((jj-1)*W+1:jj*W).^2);
                end
            case 'pk2pk' % terjedelem
                for jj=1:size(features_ff,1)
                    features_ff(jj,ii)=max(input((jj-1)*W+1:jj*W))-min(input((jj-1)*W+1:jj*W));
                end
            case 'cross' % kuszobszint keresztezeseinek szama
                features_ff(:,ii)=0;
                threshold=F{ii}{2};
                for jj=1:size(features_ff,1)
                    for kk=(jj-1)*W+2:jj*W
                        if (input(kk-1)<threshold && input(kk)>=threshold) || (input(kk-1)>threshold && input(kk)<=threshold)
                            features_ff(jj,ii)=features_ff(jj,ii)+1;
                        end
                    end
                end
            case 'crossP' % kuszobszint felfuto keresztezeseinek szama
                features_ff(:,ii)=0;
                threshold=F{ii}{2};
                for jj=1:size(features_ff,1)
                    for kk=(jj-1)*W+2:jj*W
                        if input(kk-1)<threshold && input(kk)>=threshold
                            features_ff(jj,ii)=features_ff(jj,ii)+1;
                        end
                    end
                end
            case 'crossN' % kuszobszint lefuto keresztezeseinek szama
                features_ff(:,ii)=0;
                threshold=F{ii}{2};
                for jj=1:size(features_ff,1)
                    for kk=(jj-1)*W+2:jj*W
                        if input(kk-1)>threshold && input(kk)<=threshold
                            features_ff(jj,ii)=features_ff(jj,ii)+1;
                        end
                    end
                end
            case 'peaks' % csucsok szama
                features_ff(:,ii)=0;
                threshold=F{ii}{2};
                for jj=1:size(features_ff,1)
                    for kk=(jj-1)*W+2:jj*W-1
                        if input(kk)>=input(kk-1)+threshold && input(kk)>=input(kk+1)+threshold
                            features_ff(jj,ii)=features_ff(jj,ii)+1;
                        end
                        if input(kk)<=input(kk-1)-threshold && input(kk)<=input(kk+1)-threshold
                            features_ff(jj,ii)=features_ff(jj,ii)+1;
                        end
                    end
                end
            case 'peaksP' % lokalis maximumok szama
                features_ff(:,ii)=0;
                threshold=F{ii}{2};
                for jj=1:size(features_ff,1)
                    for kk=(jj-1)*W+2:jj*W-1
                        if input(kk)>=input(kk-1)+threshold && input(kk)>=input(kk+1)+threshold
                            features_ff(jj,ii)=features_ff(jj,ii)+1;
                        end
                    end
                end
            case 'peaksN' % lokalis minimumok szama
                features_ff(:,ii)=0;
                threshold=F{ii}{2};
                for jj=1:size(features_ff,1)
                    for kk=(jj-1)*W+2:jj*W-1
                        if input(kk)<=input(kk-1)-threshold && input(kk)<=input(kk+1)-threshold
                            features_ff(jj,ii)=features_ff(jj,ii)+1;
                        end
                    end
                end
            case 'min' % minimum
                for jj=1:size(features_ff,1)
                    features_ff(jj,ii)=min(input((jj-1)*W+1:jj*W));
                end
            case 'max' % maximum
                for jj=1:size(features_ff,1)
                    features_ff(jj,ii)=max(input((jj-1)*W+1:jj*W));
                end
            otherwise
                error('Invalid feature: %s',feature);
        end
    end
    features=[features ; features_ff files{ff}{2}*ones(size(features_ff,1),1)];
end

%% ARFF fajl generalasa a Weka szamara
fp=fopen('features.arff','wt');
fprintf(fp,'@relation ''MLC''\n\n');
for ii=1:length(F)
    fprintf(fp,'@attribute F%d_%s numeric\n',ii,F{ii}{1});
end
fprintf(fp,'@attribute class {');
classes=[];
for ii=1:length(files)
    classes(end+1)=files{ii}{2};
end
classes=unique(classes);
fprintf(fp,'%d',classes(1));
for ii=2:length(classes)
    fprintf(fp,',%d',classes(ii));
end
fprintf(fp,'}\n\n');
fprintf(fp,'@data\n');
for ii=1:size(features,1)
    fprintf(fp,'%f, ',features(ii,1:end-1));
    fprintf(fp,'%d\n',features(ii,end));
end
fclose(fp);

%% dontesi fa
for ii=1:length(F)
    PredictorNames{ii}=F{ii}{1};
end
treeClassifier=ClassificationTree.fit(features(:,1:end-1),features(:,end),'PredictorNames',PredictorNames);
output=treeClassifier.predict(features(:,1:end-1));
treeClassifier.view
fprintf('Misclassifications: %d\n\n',sum(output~=features(:,end)));

%% Unico altal generalt UCF fajlbol konfiguracios tomb eloallitasa
ucffile='conf.ucf';

fpout=fopen('lsm6dsox_conf.h','wt');
fprintf(fpout,'#ifndef _CONF_H_\n');
fprintf(fpout,'#define _CONF_H_\n\n');
fprintf(fpout,'const uint8_t conf[]={\n');
fpin=fopen(ucffile,'rt');
confcount=0;
while 1
    line=fgetl(fpin);
    if ~ischar(line) % fajl vege
        break;
    elseif length(line)<2 || strcmp(line(1:2),'--') % ures sor vagy komment 
        continue;
    elseif strcmpi(line(1:4),'WAIT') % varakozas --> 0x00 cimet irunk be, ilyen regiszter ugysincs
        confcount=confcount+1;
        twait_ms=sscanf(line,'%*s %d');
        fprintf(fpout,'\t0x00, 0x%02X,\n',twait_ms);
    elseif strcmp(line(1:2),'Ac') % regiszter cim es ertek
        confcount=confcount+1;
        addr_val=sscanf(line,'Ac %x %x');
        fprintf(fpout,'\t0x%02X, 0x%02X,\n',addr_val(1),addr_val(2));
    else
        warning('unknown command\n\t%s',line)
    end
end
fclose(fpin);
fprintf(fpout,'};\n\n');
fprintf(fpout,'#define CONF_SIZE %d\n\n',confcount);
fprintf(fpout,'#endif\n');
fclose(fpout);
