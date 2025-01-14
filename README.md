Problema do grep:
	O o argumeto do grep e' pego com as aspas.

Problema do echo com cat coomo argumento:

	echo "cat README.md"

EXECUTA ESTE COMANDO NO BASH E NO SHELL
cat < aqui.txt | cat | grep c







++++++++++++ NOVA LOGICA DE PIPES E REDIRECIONADORES +++++++++++++++++
VERIFICACAO ANTES DE DIVIDIR OS COMANDOS (CMD)
TEM PIPES?
SIM: DIVIDE OS COMANDOS, TENDO COMO REFERENCIA O PIPE (|)
NAO: TEM REDIREICIONADORES?
	SIM: DIVIDE OS COMANDOS, TENDO COMO REFERENCIA OS REDIREICIONADORES
	NAO: COMANDO SIMPLES, EXECUTE.



	Casos para tratar antes de qualquer acao com a string do cmd

	 echo hello||wc -l
	 echo && 


	>> >>

	HARDOC 
	SEMPRE NO FIM DO COMANDO
	OU
	UNICO COMANDO	




	BONUS:
	minishell$> echo "'$USER'"
	Command:echo "'manandre'"$
	'manandre'