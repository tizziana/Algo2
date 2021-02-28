import random

# -------------------------------------------------------------------
# 						PRIMITIVAS DEL GRAFO	 					|
# -------------------------------------------------------------------

class Grafo:
	"""Representa un grafo(que puede ser dirigido o no dirigido), con sus primitivas:
	agregar_vertice, borrar_vertice, agregar_arista, borrar_arista, vertice_pertenece,
	vertices_conectados, obtener_obtener_peso_arista, obtener_vertices,
	obtener_vertice_aleatorio, obtener_vertices_adyacentes, obtenercantidad_vertices"""

	def __init__(self):
		"""Crea el grafo. Tiene dos diccionarios de adyacentes. En uno estan los adyacentes
		para el caso de un grafo no dirigido, y en el otro para uno dirigido. Ademas cuenta
		con un tercer diccionario que contiene los vertices como claves y datos extra, en
		una lista, como valores. Tambien tiene un contador de vertices"""
		self.adyacentes = {}
		self.adyacentes_dirigido = {}
		self.datos_vertices = {}
		self.cantidad_vertices = 0
		self.cantidad_aristas = 0

	def __len__(self):
		'''Devuelve la cantidad de vertices del grafo.'''
		return self.cantidad_vertices

	def __iter__(self):
		"""Devuelve un iterador de vertices, sin relacion entre los consecutivos."""
		return iter(self.adyacentes)

	def agregar_vertice(self, vertice, dato1 = None, dato2 = None):
		"""Agrega un nuevo vertice al grafo. En el caso de ya estar, se devuelve False,
		de lo contrario, devuelve True. Ademas guarda 2 datos, en el caso de pasarselos."""
		if self.vertice_pertenece(vertice):
			return False

		self.datos_vertices[vertice] = [dato1, dato2]
		self.cantidad_vertices += 1
		self.adyacentes[vertice] = {}
		self.adyacentes_dirigido[vertice] = {}
		return True

	def borrar_vertice(self, vertice):
		"""Borra el vertice pasado por parametro, y es borrado de sus adyacentes. Si
		no se encuentra en el grafo, devuelve None. Si es borrado devuelve el vertice."""
		if vertice not in self.adyacentes:
			return None

		for adyacente in self.adyacentes:
			if vertice in self.adyacentes[adyacente]:
				self.adyacentes[adyacente].pop(vertice, None)
				self.adyacentes_dirigido[adyacente].pop(vertice, None)
		
		self.datos_vertices.pop(vertice, None)
		self.cantidad_vertices -= 1
		self.adyacentes_dirigido.pop(vertice)
		return self.adyacentes.pop(vertice)


	def obtener_datos_vertice(self, vertice):
		"""Devuelve una lista con el formato [dato1, dato2] del vertice pasado por
		parametro."""
		return self.datos_vertices[vertice]


	def agregar_arista(self, vertice1, vertice2, peso = 1):
		"""Agrega una arista entre dos vertices (vertice1 y vertice2) con el peso pasado,
		devolviend True si la guarda.
		- En caso de que ya esten conectados, se le cambia el peso a su arista.
		- En caso de que no se pase el peso, quedara con un valor de uno.
		- Si uno de los vertices no esta en el grafo devuelve False.
		- Si el grafo no es dirigido se agregara la arista reciproca."""
		if not self.vertice_pertenece(vertice1) or not self.vertice_pertenece(vertice2):
			return False

		self.adyacentes[vertice1][vertice2] = peso
		self.adyacentes[vertice2][vertice1] = peso
		self.adyacentes_dirigido[vertice1][vertice2] = peso
		self.cantidad_aristas += 1
		return True

	
	def borrar_arista(self, vertice1, vertice2):
		"""Borra una arista entre dos vertices (vertice1 y vertice2).
		- En caso de que la arista no exista, la funcion devuelve None.
		- En caso de que uno de los vertices no se encuentre en el grafo, la
		  funcion devuelve None. Devuelve el peso de la arista en caso contrario.
		- En caso de ser no dirigido borra la arista reciproca"""
		if not vertices_conectados(self, vertice1, vertice2):
			return None

		self.adyacentes[vertice1].pop(vertice2)
		self.adyacentes[vertice2].pop(vertice1)
		self.cantidad_aristas -= 1
		return self.adyacentes_dirigido[vertice1].pop(vertice2)


	def vertice_pertenece(self, vertice):
		"""Devuelve True si el vertice pertenece al grafo, false en caso contrario"""
		return vertice in self.adyacentes.keys()


	def vertices_conectados(self, vertice1, vertice2):
		"""Devuelve True en caso de que los vertices ingresados esten conectados por
		una arista, False en caso contrario."""
		return ((self.vertice_pertenece(vertice1)) and (self.vertice_pertenece(vertice2)) and (vertice2 in self.adyacentes[vertice1]))


	def obtener_peso_arista(self, vertice1, vertice2):
		"""Devuelve el peso entre dos aristas.
		- En caso de no estar conectadas o no estar en el grafo devuelve None."""
		diccionario = self.adyacentes.get(vertice1, {})
		return int(diccionario.get(vertice2, 0))


	def obtener_vertice_aleatorio(self):
		"""Devuelve un vertice aleatorio del grafo."""
		return list(self.adyacentes.keys())[0]


	def obtener_vertices(self):
		"""Devuelve en una lista todos los vertices que posee el grafo.
		- En caso de no tener ningun vertice devuelve una lista vacia."""
		return self.adyacentes.keys()


	def obtener_vertices_adyacentes(self, vertice):
		"""Devuelve los adyacentes del vetice pasado por parametro, de
		un grafp no dirigo."""
		return self.adyacentes.get(vertice, {})


	def obtener_vertices_adyacentes_dirigido(self, vertice):
		"""Devuelve los adyacentes del vetice pasado por parametro, de
		un grafo dirigido."""
		return self.adyacentes_dirigido.get(vertice, {})


	def obtener_cantidad_vertices(self):
		"""Devuelve la cantidad de vertices que posee el grafo."""
		return self.cantidad_vertices


	def obtener_cantidad_aristas(self):
		"""Devuelve la cantidad de aristas que posee el grafo."""
		return self.cantidad_aristas


	def iterar(grafo):
		"""Iteracion del grafo con DFS (por profundidad) de forma recursiva."""
		if self.cantidad_vertices == 0: 
			return None

		visitados = []
		padre = {}
		orden = {}

		for v in self.adyacentes:
			if v not in visitados:
				dfs(self, v, visitados, orden, padre)

		return padre, orden

	def iterar_dfs(self, v, visitados, orden, padre):
		"""Funcion auxiliar de iterar"""
		visitados.append(v)
		
		for w in self.adyacentes:
			if w not in visitados:
				orden[w] = orden[v] + 1
				padre[w] = v
				dfs(self, w, visitados, orden, padre)

