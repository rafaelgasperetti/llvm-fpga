#include <iostream>
#include <lemon/list_graph.h>
#include <lemon/connectivity.h>

using namespace lemon;
using namespace std;

int main()
{	
	
	/*
	 * Grafo utilizado para testes: https://en.wikipedia.org/wiki/Strongly_connected_component
	 * Tutorial para iniciantes: http://lemon.cs.elte.hu/pub/tutorial/a00011.html
	 * Documentação arquivo connectivity.h: http://lemon.cs.elte.hu/pub/doc/1.2.3/a00428.html
	 * Documentação arquivo list_graph: http://lemon.cs.elte.hu/pub/doc/latest-svn/a00354.html
	 * Documento ensinando utilizar alguns recursos: https://lemon.cs.elte.hu/trac/lemon/browser/lemon/test/connectivity_test.cc
	 * */
	
	//Declaração do grafo direcionado.
	ListDigraph ld;
	
	//Conversor para grafo não direcionado.
	//Undirector<ListDigraph> g(ld);
	
	//Adicionando nós
	ListDigraph::Node na = ld.addNode();
	ListDigraph::Node nb = ld.addNode();
	ListDigraph::Node nc = ld.addNode();
	ListDigraph::Node nd = ld.addNode();
	ListDigraph::Node ne = ld.addNode();
	ListDigraph::Node nf = ld.addNode();
	ListDigraph::Node ng = ld.addNode();
	ListDigraph::Node nh = ld.addNode();
	
	//Definindo labels aos nós.
	ListDigraph::NodeMap<char> label(ld);
	char ch = 'A' + countNodes(ld) - 1;
	for (ListDigraph::NodeIt n(ld); n != INVALID; ++n){
		label[n] = ch--;
	}
	
	//Adicionando arcos (arestas).
	ListDigraph::Arc na_nb	= ld.addArc(na, nb);
	ListDigraph::Arc na_nb2	= ld.addArc(na, nb);
	ListDigraph::Arc nb_nc	= ld.addArc(nb, nc);
	ListDigraph::Arc nb_nc2	= ld.addArc(nb, nc);
	ListDigraph::Arc nb_ne	= ld.addArc(nb, ne);
	ListDigraph::Arc nb_nf	= ld.addArc(nb, nf);
	ListDigraph::Arc nc_nd	= ld.addArc(nc, nd);
	ListDigraph::Arc nd_nc	= ld.addArc(nd, nc);
	ListDigraph::Arc nd_nh	= ld.addArc(nd, nh);
	ListDigraph::Arc nh_nd	= ld.addArc(nh, nd);
	ListDigraph::Arc nh_ng	= ld.addArc(nh, ng);
	ListDigraph::Arc nc_ng	= ld.addArc(nc, ng);
	ListDigraph::Arc ng_nf	= ld.addArc(ng, nf);
	ListDigraph::Arc nf_ng	= ld.addArc(nf, ng);
	ListDigraph::Arc ne_na	= ld.addArc(ne, na);
	ListDigraph::Arc ne_nf	= ld.addArc(ne, nf);
	
	//Definição de pesos dos arcos (não utilizado por enquanto).
	//LengthMap length(g);

	//length[na_nb]		= 10;
	//length[na_nb2]	= 10;
	//length[nb_nc]		= 11;
	//length[nb_nc2]	= 11;
	//length[nb_ne]		= 7;
	//length[nb_nf]		= 8;
	//length[nc_nd]		= 3;
	//length[nd_nc]		= 9;
	//length[nd_nh]		= 1;
	//length[nh_nd]		= 15;
	//length[nh_ng]		= 4;
	//length[nc_ng]		= 6;
	//length[ng_nf]		= 13;
	//length[nf_ng]		= 17;
	//length[ne_na]		= 11;
	//length[ne_nf]		= 19;
	
	//Declarando o mapa dos componentes fortemente conectados e definindo qual nó pertence a qual componente.
	ListDigraph::NodeMap<int> strongComp(ld);
	int totalStrongComponents = stronglyConnectedComponents(ld,strongComp);
	
	//Exibindo um resumo dos totais do grafo.
	cout <<
		"Grafo direcionado de " << countNodes(ld) <<
		" nós e " << countArcs(ld) <<
		" arcos. Componentes fortemente conectados: " << totalStrongComponents <<
		"." <<
	endl;	
	
	//Exibindo os resultados obtidos (componentes fortemente conectados iniciados em 1).
	for (ListDigraph::ArcIt a(ld); a != INVALID; ++a){
		cout <<
			"\t" <<
			label[ld.source(a)] << " (Componente " << strongComp[ld.source(a)]+1 << ")" <<
			" -> " <<
			label[ld.target(a)] << " (Componente " << strongComp[ld.target(a)]+1 << ")" <<
		endl;
	}

	return EXIT_SUCCESS;
}
