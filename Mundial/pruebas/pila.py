
# -------------------------------------------------------------------
# 						PRIMITIVAS DE LA PILA						|
# -------------------------------------------------------------------
class Pila:
	"""Representa una pila con operaciones de apilar, desapilar y
	verificar si está vacia."""
	
	def __init__(self):
		"""Crea una pila vacía."""
		self.items = []

	def apilar(self, elemento):
		"""Apila el elemento."""
		self.items.append(elemento)

	def esta_vacia(self):
		"""Devuelve True si la lista está vacia, False si no."""
		return len(self.items) == 0

	def desapilar(self):
		"""Desapila el ultimo elemento y lo devuelve.
		- En caso de que la pila este vacia levanta una excepcion."""
		if self.esta_vacia():
			raise ValueError("La pila está vacia.")
		return self.items.pop()

	def ver_tope(self):
		"""Devuelve el tope de la pila."""
		if self.esta_vacia():
			raise Exception("La pila esta vacia")
		return self.items[-1]

	def pila_a_lista(self):
		lista = []
		while not self.esta_vacia():
			lista.append(self.desapilar())

		return lista