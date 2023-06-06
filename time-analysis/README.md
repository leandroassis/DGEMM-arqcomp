código em python pra gerar os gráficos dos resultados anteriores

##### Numero de instruções retirados de: https://www.godbolt.org

|Flag do compilador|Normal|Instru AVX| AVX+UNROLL|
|------------------|--------------------|------------------|--------------------|
|-O0|70|90|128|
|-O3|44|51|63|
|-Ofast|81|51|63|

Clock = 2.1GHZ
versao gcc = 11.3

### Métricas de avaliação
CPI = CPUtime * clock / instructions
MIPS = CPUtime/instruções
GFLOPS = (size**2 + size)/ CPUtime
