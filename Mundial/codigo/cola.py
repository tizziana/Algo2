
# -------------------------------------------------------------------
# 						PRIMITIVAS DE LA COLA						|
# -------------------------------------------------------------------

class Cola:
	"""Representa a una cola, con operaciones de encolar y
	desencolar, verificar si esta vacia, y ver primero."""

	def __init__(self):
		"""Crea una cola vacia."""
		self.items = []

	def encolar(self, elemento):
		"""Agrega el elemento como ultimo de la cola."""
		self.items.append(elemento)

	def esta_vacia(self):
		"""Devuelve True si la cola esta vacia, False si no."""
		return len(self.items) == 0

	def desencolar(self):
		"""Desencola el primer elemento y devuele su valor.
		- En caso de que la cola este vacia, levanta ValueError."""
		if self.esta_vacia():
			raise ValueError("La cola está vacia.")
		return self.items.deque()

	def ver_primero(self):
		"""Devuelvo el primer elemento de la cola."""
		if self.esta_vacia():
			raise Exception("La cola está vacía")
		return self.items[0]
