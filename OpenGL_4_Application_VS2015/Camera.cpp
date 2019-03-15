//
//  Camera.cpp
//  Lab5
//
//  Created by CGIS on 28/10/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//




#include "Camera.hpp"

namespace gps {
    
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget)
    {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraDirection = glm::normalize(cameraTarget - cameraPosition);
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
    }
    
    glm::mat4 Camera::getViewMatrix()
    {
        return glm::lookAt(cameraPosition, cameraPosition + cameraDirection , glm::vec3(0.0f, 1.0f, 0.0f));
    }

	glm::vec3 Camera::getCameraTarget()
	{
		return cameraTarget;
	}
    
    void Camera::move(MOVE_DIRECTION direction, float speed)
    {
        switch (direction) {
            case MOVE_FORWARD:
                cameraPosition += cameraDirection * speed;
                break;
                
            case MOVE_BACKWARD:
                cameraPosition -= cameraDirection * speed;
                break;
                
            case MOVE_RIGHT:
                cameraPosition += cameraRightDirection * speed;
                break;
                
            case MOVE_LEFT:
                cameraPosition -= cameraRightDirection * speed;
                break;
        }
    }
    
    void Camera::rotate(float pitch, float yaw)
    {

		cameraDirection.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		cameraDirection.y = sin(glm::radians(pitch));
		cameraDirection.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));


    }
	/*
	void Camera::setDirection(glm::vec3 newDirection, float pitch, float yaw)
	{
		glm::vec3 c(cos(pitch) * cos(yaw) * 1.5f, sin(pitch)* 1.5f, cos(pitch) * sin(yaw)* 1.5f);
		cameraPosition = glm::vec3(newDirection.x - 1.5f, newDirection.y + 3.0f, newDirection.z) - c;
		cameraTarget = newDirection + glm::vec3(0.0f,1.7f,0.0f);
		cameraDirection = glm::normalize(cameraTarget - cameraPosition);
		cameraRightDirection = glm::normalize(glm::cross(cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
	}*/

	void Camera::setDirection(glm::vec3 newDirection, float pitch, float yaw)
	{
		glm::vec3 calc(cos(pitch) * cos(yaw) * 3.5f, sin(pitch)* 3.5f, cos(pitch) * sin(yaw)* 3.5f);
		this->cameraPosition = glm::vec3(newDirection.x , newDirection.y + 4.0f, newDirection.z) - calc;
		this->cameraTarget = newDirection + glm::vec3(0.0f, 2.0f, 0.0f);
		this->cameraDirection = glm::normalize(cameraTarget - cameraPosition);
		this->cameraRightDirection = glm::normalize(glm::cross(this->cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
	}
    
}
