% Reproduce los datos de PRA v38 n7, 1988, Grbogi, Ott and Yorke
%"Roundoff-induced periodicity and the correlation dimension of chaotic attractors"

%Se itera el mapa de ikeda y se calcula el período para distintas precisiones

%% Reinicio Matlab
close all;
clear;
clc;

%% Parametros para iterar ikeda con distintas precisiones
p = 1;
B = .9;
kappa = .4;
alpha = 6;
InitCond = .1 + .1i; %Condición inicial para iterar el mapa, sirve para todas las precisiones
epsilons = logspace(-6,-2,300); %Vector de precisiones

%% Itera el mapa hasta encontrar el período para todas las precisiones

Z(1) = InitCond; %Planto la semilla, no se cual va a ser la longitud del vector

i_epsilon = 1; %Inicializo el contador de posiciones el vector de precisiones
Periods = zeros(1,length(epsilons)); %Acá van a ir guardados los períodos correspondientes a cada precisión
tic; %Arranco cronómetro
TotalElapsedTime = 0; %Inicio el registro de tiempo
for epsilon = epsilons %Por cada bucle epsilon va tomando los valores del vector epsilons
    
    n = 1; %Índice con la posición del mapa
    Position = 1; %Acá van los índices de las posiciones en el mapa que son iguales a la última posición
    while length(Position) < 2; %Mientras el vector que contiene las posiciones tenga un solo índice
        
        Z(n+1) = p + B*Z(n)*exp(1i*(kappa-alpha/(1+abs(Z(n))^2))); %Hago una iteración del mapa
        
        Z(n+1) = epsilon*round(Z(n+1)/epsilon); %Redondeo el resultado
        
        Position = find(Z==Z(n+1)); %Busco todas las posiciones en donde se repita el último valor
        
        n = n+1; %Incremento índice del mapa
    end
    
    Periods(i_epsilon) = Position(2) - Position(1) + 1; %Calculo y guardo el período

    PartialElapsedTime = toc - TotalElapsedTime; %Calcula el tiempo parcial
    TotalElapsedTime = toc; %Registra el tiempo total
    disp(['epsilons(' num2str(i_epsilon) ') = ' num2str(epsilon) '; Tiempo total = ' num2str(TotalElapsedTime) '; Tiempo parcial = ' num2str(PartialElapsedTime)]); %Muestra un cartel con información del tiempo
    
    i_epsilon = i_epsilon + 1; %Incremento la posición en el vector de precisiones
    
end

SpectedPeriod=epsilons.^-0.835; %.0835 es la mitad de la dimensión de correlación

FittedCorrelationDimention=polyfit(log10(epsilons),log10(Periods),1); %Halla la dimensión de correlación para los puntos hallados
FittedPeriod=epsilons.^FittedCorrelationDimention(1); %Halla los períodos promediados
%% Guardo los resultados

save('IkedaGrebogiResults','epsilons','Periods','TotalElapsedTime','SpectedPeriod','FittedCorrelationDimention','FittedPeriod');
%% Muestro resultados

% Create figure
figure1 = figure;

% Create axes
axes1 = axes('Parent',figure1,'XScale','log','YScale','log');
box(axes1,'on');
grid(axes1,'on');
hold(axes1,'on');
xlabel('\epsilon');
ylabel('m');

% Create semilogy
loglog(epsilons,Periods,'Marker','.','LineStyle','none');

% Agrego el período esperado y el calculado
hold on;
loglog(epsilons,SpectedPeriod);
loglog(epsilons,FittedPeriod);
