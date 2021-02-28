from grafo import Grafo
from heap import Heap
from cola import Cola
from pila import Pila

# -----------------------------------------------------------------------
# 						FUNCIONES DE LA BIBLIOTECA						|
# -----------------------------------------------------------------------

def camino_minimo(grafo, desde, hasta):
	"""Devuelve una lista con el camino minimo entre una sede y otra.
	En la lista estaran todas las sedes por las que se debe pasar para
	llegar a destino."""
	padre, distancia = dijkstra(grafo, desde)
	camino = []
	punto_actual = hasta

	while True:
		camino.append(punto_actual)
		if punto_actual == desde: break
		punto_actual = padre[punto_actual]
	
	camino.reverse()
	return camino, distancia[hasta]

	
def dijkstra(grafo, origen):
	"""Devuelve el camino más corto dado un vértice de origen al resto
	de vértices en un grafo con pesos en cada arista."""
	padre = {}
	visitados = []
	distancia = {vertice: float("inf") for vertice in grafo.obtener_vertices()}

	distancia[origen] = 0
	padre[origen] = None
	visitados.append(origen)

	heap = Heap()
	heap.encolar(origen, distancia[origen])

	while not heap.esta_vacio():
		vertice, peso = heap.desencolar()
		for adyacente in grafo.obtener_vertices_adyacentes(vertice):
			peso_arista = grafo.obtener_peso_arista(vertice, adyacente)
			if distancia[adyacente] > (peso_arista + distancia[vertice]):
				padre[adyacente] = vertice
				distancia[adyacente] = distancia[vertice] + peso_arista
				heap.encolar(adyacente, distancia[adyacente])

	return padre, distancia

#-------------------------------------------------------------------------

def viajante_aproximado(grafo, desde):
	"""Devuelve una lista con el recorrido desde el vertice ingresado
	por parametro, pasando por todos los vertices del grafo una sola
	vez, hasta volver al inicio, de una forma aproximada."""
	return  _viajante_aproximado(grafo, desde, [desde], 0)

def _viajante_aproximado(grafo, vertice, recorrido, costo):
	if (len(recorrido) == grafo.obtener_cantidad_vertices()):
		recorrido.append(recorrido[0])
		costo += grafo.obtener_peso_arista(vertice, recorrido[0])
		return recorrido, costo

	menor_distancia = ""
	peso_minimo = float("Inf")

	for adyacente in grafo.obtener_vertices_adyacentes(vertice):
		if adyacente not in recorrido:
			peso = float(grafo.obtener_peso_arista(vertice, adyacente))
			if peso < peso_minimo:
				menor_distancia = adyacente
				peso_minimo =  peso

	recorrido.append(menor_distancia)
	costo += grafo.obtener_peso_arista(vertice, menor_distancia)
	return _viajante_aproximado(grafo, menor_distancia, recorrido, costo)

#-------------------------------------------------------------------------

def viajante(grafo, desde):
	"""Devuelve una lista con el recorrido desde el vertice ingresado por 
	parametro, pasando por todos los vertices del grafo una sola vez,
	hasta volver al inicio, de una forma exacta."""
	visitados = []
	costo_minimo = 999
	recorrido, costo = _viajante(grafo, desde, desde, visitados, 0, costo_minimo)
	recorrido.reverse()
	return recorrido, costo

def _viajante(grafo, origen, vertice, visitados, costo_relativo, costo_minimo):
	final = []
	
	visitados.append(vertice)
	if len(visitados) > 1:
		costo_relativo += grafo.obtener_peso_arista(visitados[-2], vertice)
	
	if len(visitados) == grafo.obtener_cantidad_vertices():
		costo_relativo += grafo.obtener_peso_arista(visitados[-1], origen)
		visitados.append(origen)
		return visitados, costo_relativo

	if costo_relativo > costo_minimo:
		return final, costo_minimo

	for w in grafo.obtener_vertices_adyacentes(vertice):
		if w in visitados:
			continue
		recorrido, costo =  _viajante (grafo, origen, w, visitados[:], costo_relativo, costo_minimo) 
		if((len(recorrido) + 1) != grafo.obtener_cantidad_vertices()) and costo < costo_minimo:
			costo_minimo = costo
			final = recorrido

	return final, costo_minimo

#-------------------------------------------------------------------------

def orden_topologico(grafo):
	"""Devuelve una lista con el orden topologico con el que se debe
	recorrer el grafo pasado por parametro. En caso de no existir
	devuelve None."""
	if not grafo:
		return None
	
	visitados = set()
	pila = Pila()

	for vertice in grafo.obtener_vertices():
		if vertice not in visitados:
			orden_topologico_dfs(grafo, vertice, pila, visitados)

	return pila.pila_a_lista()

def orden_topologico_dfs(grafo, vertice, pila, visitados):
	visitados.add(vertice)

	for adyacente in grafo.obtener_vertices_adyacentes_dirigido(vertice):
		if adyacente not in visitados:
			orden_topologico_dfs(grafo, adyacente, pila, visitados)

	pila.apilar(vertice)

#-------------------------------------------------------------------------

def arbol_tendido_minimo(grafo):
	"""Devuelve un nuevo grafo que representa un arbol de tendido minimo.
	- Pre-condicion: recibe un grafo conexo."""
	inicio = grafo.obtener_vertice_aleatorio()
	visitados = set()
	visitados.add(inicio)
	heap = Heap()

	for adyacente in grafo.obtener_vertices_adyacentes(inicio):
		heap.encolar((inicio, adyacente), grafo.obtener_peso_arista(inicio, adyacente))

	arbol = Grafo()	
	for vertice in grafo.obtener_vertices():
		arbol.agregar_vertice(vertice)

	while not heap.esta_vacio():
		[(vertice, adyacente), peso_arista] = heap.desencolar()
		if adyacente in visitados:
			continue
		arbol.agregar_arista(vertice, adyacente, peso_arista)
		visitados.add(adyacente)

		for vecino in grafo.obtener_vertices_adyacentes(adyacente):
			heap.encolar((adyacente, vecino), grafo.obtener_peso_arista(adyacente, vecino))
	
	return arbol
