
# -------------------------------------------------------------------
# 						PRIMITIVAS DEL HEAP							|
# -------------------------------------------------------------------

class Heap:
	"""Representa un heap de min con operaciones de encolar, desencolar,
	ver_maximo, cantidad y verificar si está vacio."""

	def __init__(self):
		"""Crea un heap vacio."""
		self.items = []
		self.cantidad = 0

	def encolar(self, elemento, peso):
		"""Agrega el elemento como ultimo del heap."""
		self.items.append([elemento, peso])
		self.cantidad += 1
		
		self.upheap(self.cantidad - 1)

	def esta_vacio(self):
		"""Devuelve True si el heap esta vacio, False si no."""
		return self.cantidad == 0
		
	def desencolar(self):
		"""Desencola el primer elemento y devuelve su valor.
		- En caso de que el heap este vacio, levanta ValueError."""
		if self.esta_vacio():
			raise ValueError("El heap esta vacio")

		elemento = self.items.pop(0)
		self.cantidad -= 1
		self.downheap(self.cantidad, 0)
		return elemento

	def cantidad(self):
		"""Devuelve la cantidad de elementos que tiene el heap."""
		return self.cantidad

	def ver_maximo(self):
		"""Devuelve el elemento maximo del heap."""
		return self.items[0]

	def upheap(self, posicion):
		if not posicion: return
		pos_padre = int((posicion - 1) / 2)

		if self.items[(posicion)][1] > self.items[(pos_padre)][1]:
			return
		
		self.items[posicion], self.items[pos_padre] = self.items[pos_padre], self.items[posicion] 
		self.upheap(pos_padre)
	

	def downheap(self, tamanio, posicion):
		if (posicion >= tamanio): return
		pos_h_izq = 2 * posicion + 1
		pos_h_der = pos_h_izq + 1
		pos_max = posicion

		if ((pos_h_izq < tamanio) and (self.items[pos_h_izq][1] < self.items[pos_max][1])):
			pos_max = pos_h_izq

		if ((pos_h_der < tamanio) and (self.items[pos_h_der][1] < self.items[pos_max][1])):
			pos_max = pos_h_der

		if (posicion != pos_max):
			self.items[posicion], self.items[pos_max] = self.items[pos_max], self.items[posicion]
			self.downheap(tamanio, pos_max)

		self.downheap(tamanio, posicion + 1)