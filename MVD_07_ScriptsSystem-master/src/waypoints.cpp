#include "waypoints.h"
#include "extern.h"


void waypoints::init() 
{
	//inicializo el primer waypoint
	current_waypoint = 0;

	//inicializamos variables
	first = true;
	first_seguir = true;
	girar = false;
	seguir = 3;
	waypo = false;
	frontal = false;

	//añado los seis waypoints k originalmente deberian leerse de JSON
	Waypoints.push_back(lm::vec3(-10, 0, -10));
	Waypoints.push_back(lm::vec3(-25, 0, -25));
	Waypoints.push_back(lm::vec3(-40, 0, -10));
	Waypoints.push_back(lm::vec3(-40, 0, -40));
	Waypoints.push_back(lm::vec3(-25, 0, -25));
	Waypoints.push_back(lm::vec3(-10, 0, -40));

	//cogemos el transform del dueño y el player y la posicion del player
	transform = &ECS.getComponentFromEntity<Transform>(owner_);
	player = &ECS.getComponentFromEntity<Transform>(player_id);
	playerPos = player->position();
}

void waypoints::update(float dt) 
{
	//front vector
	lm::vec3 front = transform->front();
	//front vector normalizado
	lm::vec3 front_norm = front.normalize();
	//guardamos posicion original
	lm::vec3 initialPosition = transform->position();
	
	//calculamos la distancia hacia el waypoint actual
	float distance = lm::vec3(Waypoints[current_waypoint].x - initialPosition.x, 0 , Waypoints[current_waypoint].z - initialPosition.z).length();
	//posicion del jugador
	lm::vec3 playerPosition = lm::vec3(player->m[12], 0, player->m[14]);
	//distancia con el jugador
	float playerDist = lm::vec3(player->m[12] - initialPosition.x, 0, player->m[14] - initialPosition.z).length();

	//cambiar de waypoint cuando llegamos al destino
	if (distance < 1)
	{
		current_waypoint++;
		if (current_waypoint >= Waypoints.size()) {
			current_waypoint = 0;
		}
		first = true;
	}

	float velocidad = 1.0;
	float vel_rot = 1.0;
	lm::vec3 distTarget;

	//miramos si el player esta detro de la area (es una area triangular con un vertice en el enemigo) frontal
	//la posicion del player y del enemigo cogiendo solo x y z
	lm::vec2 v = lm::vec2(player->position().x, player->position().z);
	lm::vec2 v0 = lm::vec2(transform->position().x, transform->position().z);

	//calculamos si lo ve frontalmente
	lm::vec2 pos_frontal = lm::vec2(v0.x + front_norm.x , v0.y + front_norm.z );
	//calculamos los puntos de la derecha y izquierda, lo hacemos en vec 2 pk la altura nos da igual
	lm::vec2 derecha = pos_frontal * sin(-45);
	lm::vec2 izquierda = pos_frontal * sin(45);


	//calculamos los bordes del area
	lm::vec2 v1 = izquierda - v0;
	lm::vec2 v2 = derecha - v0;

	//miramos si esta dentro del area sigueindo la formula
	float a1 = ((v.x * v2.y - v.y * v2.x) - (v0.x * v2.y - v0.y * v2.x)) / (v1.x * v2.y - v1.y * v2.x);
	float a2 = ((v.x * v1.y - v.y * v1.x) - (v0.x * v1.y - v0.y * v1.x)) / (v1.x * v2.y - v1.y * v2.x);

	//miramos si esta en el area del triangulo si las dos a son positivas
	if (a1 > 0 && a2 > 0) {
		frontal = true;
	}
	//fuera del triangulo
	if (a1 <= 0 || a2 <= 0 || playerDist > 10) {
		frontal = false;
	}
	
	//si tiene k seguir al personaje
	if (playerDist < seguir || frontal) {
		if (first_seguir) {
			first = true;
			first_seguir = false;
		}
		velocidad = 2.0;
		vel_rot = 2.0;
		distTarget = lm::vec3(player->m[12] - initialPosition.x, 0, player->m[14] - initialPosition.z);
		seguir = 6;
		waypo = false;
	}
	//si va a por los waypoints
	else if (playerDist >= seguir ) {
		if (seguir == 6) {
			first = true;
		}
		first_seguir = true;
		//distancia hasta el target
		distTarget = lm::vec3(Waypoints[current_waypoint].x - initialPosition.x, 0, Waypoints[current_waypoint].z - initialPosition.z);
		seguir = 3;
		waypo = true;
	}


	//rotamos, buscamos el angulo para girar
	distTarget.normalize();
	float aux = distTarget.dot(front_norm);
	float angle = acos(aux) * 180 / 3.1415;


	//calculamos el eje de rotacion, en este caso el up del objeto
	lm::vec3 rot_axis = lm::vec3(transform->m[4], transform->m[5], transform->m[6]);

	//elegimos el lado en el que gira mirando si el angulo a aumentado o disminuido en el frame anterior
	if (girar) {
		if (angle > anguloAux) {
			vel_rotate = dt;
		}
		else{
			vel_rotate = -dt;
		}
		girar = false;
	}


	//empezamos con el giro
	if (first) {
		anguloAux = angle;
		vel_rotate = -dt;
		first = false;
		girar = true;
		playerPos = playerPosition;
	}

	//ponemos el first a true cuando sigue el jugador se mueve
	if (changePos(playerPosition) && !waypo) {
		first = true;
	}
	
	//rotamos el objeto cuando hace los waypoints
	if (angle > 5 ) 
	{
		transform->rotateLocal(vel_rotate * vel_rot, rot_axis);
	}

	//cuando no rota movemos hacia el lugar
	if(angle <= 5)
	{
		first = true;
		transform->translate(front_norm.x * dt * velocidad, 0, front_norm.z * dt * velocidad);
	}	

}

//false si la posicion es la misma, true si cambia
bool waypoints::changePos(lm::vec3 pos) {
	if (pos.x == playerPos.x && pos.z == playerPos.z) {
		return false;
	}
	return true;
}
