import pandas as pd
from math import sqrt


class Resultados():
    
    def __init__(self, path : str):
        self.name = path
        self.dataframe = pd.read_csv(path, sep=";")
        self.mediaFLOPS = []
        self.mediaTempo = []

    def calculaFLOPS(self) -> list:
        '''
            Calcula o FLOPS médio para cada tamanho de matriz em cada função.

            self.mediaFLOPS[0][0] = média de FLOPS da função 0 para o tamanho de matriz 0
            0 - 4096X4096
            1 - 5120X5120
            2 - 6144X6144
            3 - 7168X7168
            4 - 8192X8192
        '''

        tamanhos = self.dataframe["Tamanho da matriz"].unique() # pega todos os tamanhos de matriz
        # para cada coluna (função)
        for i in self.dataframe.columns[1:]:
            self.mediaFLOPS.append([])
            self.mediaTempo.append([])
            # para cada tamanho de matriz
            for j in tamanhos:
                # tira média das linhas que possuem tamanho de matriz = j e estão na coluna i
                media_tempo_por_tamanho = self.dataframe[self.dataframe["Tamanho da matriz"] == j][i].mean()
                std_tempo_por_tamanho = self.dataframe[self.dataframe["Tamanho da matriz"] == j][i].std()

                if "resultadosCUDA" in self.name:
                    media_tempo_por_tamanho /= 1000
                    std_tempo_por_tamanho /= 1000
                
                self.mediaTempo[-1].append((media_tempo_por_tamanho, std_tempo_por_tamanho))

                tamanho : float = sqrt(float(j))
                flops = tamanho**3/media_tempo_por_tamanho
                std_flops = flops*sqrt((1/tamanho**3) + (std_tempo_por_tamanho/media_tempo_por_tamanho)**2)

                self.mediaFLOPS[-1].append((flops/1e9, std_flops/1e9))

        return self.mediaFLOPS
    
    def getFLOPSValues(self, tamanho : str) -> list:
        '''
            Retorna uma lista com os valores de FLOPS (mean, std) que cada tamanho obteve para a função passado por parâmetro.
        '''
        media = []
        std = []

        match tamanho:
            case "normal":
                for i in self.mediaFLOPS[0]:
                    media.append(i[0])
                    std.append(i[1])
                return media, std
            case "avx":
                for i in self.mediaFLOPS[1]:
                    media.append(i[0])
                    std.append(i[1])
                return media, std
            case "unroll":
                for i in self.mediaFLOPS[2]:
                    media.append(i[0])
                    std.append(i[1])
                return media, std
            case "block":
                for i in self.mediaFLOPS[3]:
                    media.append(i[0])
                    std.append(i[1])
                return media, std
            case _:
                print("Função inválida.")
                return []
    
    def getTempoValues(self, tamanho : str):
        '''
            Retorna listas com os valores de tempo (mean, std) que cada tamanho obteve para a função passado por parâmetro.
        '''
        media = []
        std = []
        match tamanho:
            case "normal":
                for i in self.mediaTempo[0]:
                    media.append(i[0])
                    std.append(i[1])
                return media, std
            case "avx":
                for i in self.mediaTempo[1]:
                    media.append(i[0])
                    std.append(i[1])
                return media, std
            case "unroll":
                for i in self.mediaTempo[2]:
                    media.append(i[0])
                    std.append(i[1])
                return media, std
            case "block":
                for i in self.mediaTempo[3]:
                    media.append(i[0])
                    std.append(i[1])
                return media, std
            case _:
                print("Função inválida.")
                return []