
# -----------------------------------------------------------------------
#							TRABAJO PRACTICO 3							|
# Grupo: G20															|
# Integrantes: Dvorkin, Camila (101109) - Mazza Reta, Tizziana (101715) |
# Corrector: Martin Buchwald											|
# -----------------------------------------------------------------------

import csv
import sys
from grafo import Grafo
from biblioteca import *

# -----------------------------------------------------------------------
# 								COMANDOS								|
# -----------------------------------------------------------------------

def main():
	"""Funcion principal. Se encarga de la lectura de parametros en la 
	inea de comando y del llamado de la funcion a ejecutar segun cual
	sea ese comando."""
	if len(sys.argv) != 3:
		raise TypeError("Cantidad de parametros pasados erronea.")

	try:
		with open(sys.argv[1], "r") as sedes:	
			lector_sedes = csv.reader(sedes)
			grafo_sedes = Grafo()
			
			cantidad_sedes = int(next(lector_sedes,[0])[0])
			for i in range(cantidad_sedes):

				sede, latitud, longitud = next(lector_sedes)
				grafo_sedes.agregar_vertice(sede, latitud, longitud)

			cantidad_aristas = int(next(lector_sedes,[0])[0])
			for i in range(cantidad_aristas):
				sede_1, sede_2, tiempo  = next(lector_sedes)
				grafo_sedes.agregar_arista(sede_1, sede_2, tiempo)


	except OSError:
		print("No se pudo abrir el archivo {}".format(sys.argv[1]))
		return


	comandos = {"ir": camino_minimo_entre_sedes,
				"viaje": viaje,
				"itinerario": itinerario,
				"reducir_caminos": reducir_caminos}


	while True:
		try:
			linea = input()

		except EOFError as error:
			break


		if ',' in linea:
			comando, param = linea.split(",")
			comando_= comando.split()
			comando = comando_[0]
			opcion = " ".join(comando_[1:])
			
			try:
				comandos[comando](grafo_sedes, opcion, param.lstrip().rstrip(), sys.argv[2])
			except KeyError:
				print("'{}'' no pertenece a un comando valido".format(comando))
				
		else:
			comando, param = linea.split()
			try:
				comandos[comando](grafo_sedes, param.lstrip().rstrip(), sys.argv[2])

			except KeyError:
				print("'{}'' no pertenece a un comando valido".format(comando))


#-----------------------------------------------------------------------------

def camino_minimo_entre_sedes(grafo_sedes, desde, hasta, mapa_kml):
	"""Imprime el camino minimo entre las sedes recibidas(desde -> .. -> hasta),
	y el costo que eso requiere."""
	if not grafo_sedes.vertice_pertenece(desde):
		raise NameError('{} no pertenece a las sedes del mundial de tejo'.format(desde))
	if not grafo_sedes.vertice_pertenece(hasta):
		raise NameError('{} no pertenece a las sedes del mundial de tejo'.format(hasta))
	
	recorrido, costo = camino_minimo(grafo_sedes, desde, hasta)
	imprimir_recorrido(grafo_sedes, recorrido, costo)
	crear_kml(grafo_sedes, "ir {}, {}".format(desde, hasta), recorrido, mapa_kml)

#-----------------------------------------------------------------------------

def viaje(grafo_sedes, solucion, origen, mapa_kml):
	"""Imprime el orden de las sedeses a visitar para verlas todas una vez,
	y volver a la cuidad desde la cual se partio. La solucion devuelta es
	optima o aproximada segun el segundo parametro pasado(solucion)."""
	if not grafo_sedes.vertice_pertenece(origen):
		raise NameError("{} no pertenece a las sedes del mundial de tejo".format(origen))
	
	if "optimo" == solucion: recorrido, costo = viajante(grafo_sedes, origen)
	elif "aproximado" == solucion: recorrido, costo = viajante_aproximado(grafo_sedes, origen)
	else: raise NameError('"Viaje {}" no pertenece a un comando valido'.format(solucion))
	imprimir_recorrido(grafo_sedes, recorrido, costo)
	crear_kml(grafo_sedes, "viaje {}, {}".format(solucion, origen), recorrido, mapa_kml)

#------------------------------------------------------------------------------

def itinerario(grafo_sedes, archivo_recomendaciones, mapa_kml):
	"""Recibe un itinerario que nos indica cuales son las sedes que debemos
	visitar antes que otras, y con ellas hace un nuevo grafo con el que 
	llama a orden topologico, para luego imprimir su reccorrido segun el
	itinerario pasado."""

	try:	
		with open(archivo_recomendaciones, "r") as recomendaciones:
			lector = csv.reader(recomendaciones)

			grafo_recomendaciones = Grafo()

			for sede_1,sede_2 in lector:
			
				if not grafo_recomendaciones.vertice_pertenece(sede_1):
					grafo_recomendaciones.agregar_vertice(sede_1)
			
				if not grafo_recomendaciones.vertice_pertenece(sede_2):
					grafo_recomendaciones.agregar_vertice(sede_2)

				if grafo_sedes.vertices_conectados(sede_1, sede_2):
					grafo_recomendaciones.agregar_arista(sede_1, sede_2, grafo_sedes.obtener_peso_arista(sede_1, sede_2))
	except OSError:
		print("No se pudo abrir el archivo {}".format(archivo_recomendaciones))
		return			
	
	recorrido = orden_topologico(grafo_recomendaciones)
	if (recorrido): imprimir_recorrido(grafo_sedes, recorrido, 0)
	crear_kml(grafo_sedes, "itinerario {}".format(archivo_recomendaciones), recorrido, mapa_kml)


#-----------------------------------------------------------------------------

def reducir_caminos(grafo_sedes, archivo_destino, mapa_kml):
	_reducir_caminos(grafo_sedes, archivo_destino)

def _reducir_caminos(grafo_sedes, archivo_destino):
	"""Recibe un archivo en el cual escribe el camino minimo entre todas
	las sedes del mundial, e imprime el costo total de este camino."""
	recorrido = arbol_tendido_minimo(grafo_sedes)
	peso_recorrido = 0

	visitados = {sede: [] for sede in recorrido.obtener_vertices()}

	with open(archivo_destino, "w") as destino:
		destino_csv = csv.writer(destino)

		destino_csv.writerow(str(recorrido.obtener_cantidad_vertices()).split(','))

		for sede in recorrido.obtener_vertices():
			latitud, longitud = grafo_sedes.obtener_datos_vertice(sede)
			destino_csv.writerow((sede, latitud, longitud))

		destino_csv.writerow(str(recorrido.obtener_cantidad_aristas()).split(','))
		
		for sede in recorrido.obtener_vertices():
			for vecino in recorrido.obtener_vertices_adyacentes(sede):
				if sede not in visitados[vecino]:
					peso_arista = recorrido.obtener_peso_arista(sede, vecino)
					peso_recorrido += peso_arista
					destino_csv.writerow((sede, vecino, peso_arista))
					visitados[sede].append(vecino)

	print("Peso total:", peso_recorrido)

# -----------------------------------------------------------------------
# 						FUNCIONES AUXILIARES							|
# -----------------------------------------------------------------------

def imprimir_recorrido(grafo_sedes, recorrido, costo):
	"""Imprime el recorrido pasado por parametro, con una flecha(->)
	entre cada sede, y el costo de este."""
	suma_costo = costo

	for i in range(len(recorrido) - 1):
			print(recorrido[i], '->', end = ' ')	
			if not costo and i > 0:
				suma_costo += grafo_sedes.obtener_peso_arista(recorrido[i-1], recorrido[i])

	if recorrido != []: print(recorrido[-1])
	if not costo: suma_costo += grafo_sedes.obtener_peso_arista(recorrido[-2], recorrido[-1])
	print("Costo total:", suma_costo)



def crear_kml(grafo, comando, recorrido, archivo_mapa):
	"""Crea un archivo de tipo KML con las coordenadas de cada sede
	en el recorrido dado, y la union de estas."""
	try:
		with open(archivo_mapa, "w") as mapa:

			mapa.writelines('<?xml version="1.0" encoding="UTF-8"?>\n''<kml xmlns="http://earth.google.com/kml/2.1">\n')
			mapa.writelines('\t<Document>\n''\t\t<name>{}</name>\n\n'.format(comando))

			for i in range(len(recorrido)):
				mapa.write('\t\t<Placemark>\n')
				mapa.write('\t\t\t<name>{}</name>\n'.format(recorrido[i]))
				mapa.write('\t\t\t<Point>\n')
				mapa.write('\t\t\t\t<coordinates>{}</coordinates>\n'.format(", ".join(grafo.obtener_datos_vertice(recorrido[i]))))
				mapa.write('\t\t\t</Point>\n')
				mapa.write('\t\t</Placemark>\n')

			for i in range(len(recorrido)-1):
				mapa.write('\t\t<Placemark>\n')
				mapa.write('\t\t\t<LineString>\n')
				mapa.write('\t\t\t\t<coordinates>{} {}</coordinates>\n'.format(", ".join(grafo.obtener_datos_vertice(recorrido[i])), ", ".join(grafo.obtener_datos_vertice(recorrido[i+1]))))
				mapa.write('\t\t\t</LineString>\n')
				mapa.write('\t\t</Placemark>\n')

			mapa.write('\t</Document>\n')
			mapa.write('</kml>\n')


	except OSError:
		print("No se pudo abrir el archivo {}".format(archivo_mapa))



main()
