código em python pra gerar os gráficos dos resultados anteriores

##### Numero de instruções retirados de: https://www.godbolt.org com flag -mavx -lrt -Wall, versão gcc x86 11.3

|Flag do compilador|Normal|Instru AVX| AVX+UNROLL|AVX+BLOCK|
|------------------|--------------------|------------------|--------------------|--------------------|
|-O0|70|90|128|203|
|-O3|44|51|63|147|
|-Ofast|81|51|63|147|



### Métricas de avaliação
Núm de instruções para cada flag
GFLOPS = (size**2 + size)/ CPUtime


