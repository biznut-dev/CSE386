/****************************************************
 * 2016-2021 Eric Bachmann and Mike Zmuda
 * All Rights Reserved.
 * NOTICE:
 * Dissemination of this information or reproduction
 * of this material is prohibited unless prior written
 * permission is granted.
 ****************************************************/

#include "light.h"
#include "io.h"

/**
 * @fn	color ambientColor(const color &mat, const color &light)
 * @brief	Computes the ambient color produced by a single light at a single point.
 * @param	mat  	Ambient material property.
 * @param	lightAmbient	Light's ambient color.
 * @return	Ambient color.
  */

color ambientColor(const color &mat, const color &lightAmbient) {
	return glm::clamp(mat * lightAmbient, 0.0, 1.0);
}

/**
 * @fn	color diffuseColor(const color &mat, const color &light, const dvec3 &l, const dvec3 &n)
 * @brief	Computes diffuse color produce by a single light at a single point.
 * @param	mat		 	Material.
 * @param	lightDiffuse	 	The light color.
 * @param	l		 	Light vector.
 * @param	n		 	Normal vector.
 * @return	Diffuse color.
 */

color diffuseColor(const color &mat, const color &lightDiffuse,
					const dvec3 &l, const dvec3 &n) {
	double dp = glm::max(0.0, glm::dot(l, n));
	return glm::clamp(mat * lightDiffuse * dp, 0.0, 1.0);
}

/**
 * @fn	color specularColor(const color &mat, const color &light, double shininess, 
 *							const dvec3 &r, const dvec3 &v)
 * @brief	Computes specular color produce by a single light at a single point.
 * @param	mat		 	Material.
 * @param	lightSpecular	 	The light's color.
 * @param	shininess	Material shininess.
 * @param	r		 	Reflection vector.
 * @param	v		 	Viewing vector.
 * @return	Specular color.
 */

color specularColor(const color &mat, const color &lightSpecular,
					double shininess,
					const dvec3 &r, const dvec3 &v) {
	return glm::clamp( mat * lightSpecular * std::pow(glm::clamp(glm::dot(v, r), 0.0, 1.0), shininess) , 0.0, 1.0 );
}

/**
 * @fn	color totalColor(const Material &mat, const LightColor &lightColor, 
 *						const dvec3 &viewingDir, const dvec3 &normal, 
 *						const dvec3 &lightPos, const dvec3 &intersectionPt, 
 *						bool attenuationOn, const LightAttenuationParameters &ATparams)
 * @brief	Color produced by a single light at a single point.
 * @param	mat			  	Material.
 * @param	lightColor	  	The light's color.
 * @param	v	  			The v vector.
 * @param	n   		  	Normal vector.
 * @param	lightPos	  	Light position.
 * @param	intersectionPt	(x,y,z) of intersection point.
 * @param	attenuationOn 	true if attenuation is on.
 * @param	ATparams	  	Attenuation parameters.
 * @return	Color produced by a single light at a single point.
 */
 
color totalColor(const Material &mat, const LightColor &lightColor,
				const dvec3 &v, const dvec3 &n,
				const dvec3 &lightPos, const dvec3 &intersectionPt,
				bool attenuationOn, 
				const LightATParams &ATparams) {

	color total;
	dvec3 lightVec = glm::normalize(lightPos - intersectionPt);
	double lightDistance = glm::distance(lightPos, intersectionPt);
	double AT = ATparams.factor(lightDistance);
	dvec3 reflectionVec = 2 * glm::dot(lightVec, n) * n - lightVec;

	color ambColor = ambientColor(mat.ambient, lightColor.ambient);
	color diffColor = diffuseColor(mat.diffuse, lightColor.diffuse, lightVec, n);
	color specColor = specularColor(mat.specular, lightColor.specular, mat.shininess, reflectionVec, v);

	if (attenuationOn)
	{
		total = ambColor + AT * (diffColor + specColor);
	}
	else
	{
		total = ambColor + diffColor + specColor;
	}
	
	return glm::clamp(total, 0.0, 1.0);
}

/**
 * @fn	color PositionalLight::illuminate(const dvec3 &interceptWorldCoords, 
 *										const dvec3 &normal, const Material &material, 
 *										const Frame &eyeFrame, bool inShadow) const
 * @brief	Computes the color this light produces in RAYTRACING applications.
 * @param	interceptWorldCoords	(x, y, z) at the intercept point.
 * @param	normal				The normal vector.
 * @param	material			The object's material properties.
 * @param	eyeFrame			The coordinate frame of the camera.
 * @param	inShadow			true if the point is in a shadow.
 * @return	The color produced at the intercept point, given this light.
 */

color PositionalLight::illuminate(const dvec3& interceptWorldCoords,
									const dvec3& normal,
									const Material& material,
									const Frame& eyeFrame, bool inShadow) const {

	color interceptColor = color(0, 0, 0);

	if (isOn)
	{
		if (inShadow)
		{
			interceptColor = ambientColor(material.ambient, lightColor.ambient);
		}
		else
		{
			interceptColor = totalColor(material, lightColor, glm::normalize(eyeFrame.origin - interceptWorldCoords), 
				normal, actualPosition(eyeFrame), interceptWorldCoords, attenuationIsTurnedOn, atParams);
		}
	}

	return interceptColor;
}

/*
* @fn	PositionalLight::actualPosition(const Frame& eyeFrame) const
* @brief	Returns the global world coordinates of this light.
* @param	eyeFrame	The camera's frame
* @return	The global world coordinates of this light. This will the light's
*			raw position. Or, it will be the position relative to the camera's
*			frame (transformed into the world coordinate frame).
*/
dvec3 PositionalLight::actualPosition(const Frame& eyeFrame) const {
	return isTiedToWorld ? pos : eyeFrame.toWorldCoords(pos);
}

/**
 * @fn	color SpotLight::illuminate(const dvec3 &interceptWorldCoords, 
 *									const dvec3 &normal, const Material &material, 
 *									const Frame &eyeFrame, bool inShadow) const
 * @brief	Computes the color this light produces in raytracing applications.
 * @param	interceptWorldCoords				The surface properties of the intercept point.
 * @param	normal					The normal vector.
 * @param	material			The object's material properties.
 * @param	eyeFrame			The coordinate frame of the camera.
 * @param	inShadow			true if the point is in a shadow.
 * @return	The color produced at the intercept point, given this light.
 */

color SpotLight::illuminate(const dvec3 &interceptWorldCoords,
							const dvec3 &normal,
							const Material &material,
							const Frame &eyeFrame, bool inShadow) const {

	color interceptColor = color(0, 0, 0);

	if (inCone(actualPosition(eyeFrame), spotDir, fov, interceptWorldCoords))
	{
		interceptColor = PositionalLight::illuminate(interceptWorldCoords, normal, material, eyeFrame, inShadow);
	}

	return interceptColor;
}

/**
* @fn	void setDir (double dx, double dy, double dz)
* @brief	Sets the direction of the spotlight.
* @param	dx		x component of the direction
* @param	dy		y component of the direction
* @param	dz		z component of the direction
*/

void SpotLight::setDir(double dx, double dy, double dz) {
	spotDir = glm::normalize(dvec3(dx, dy, dz));
}

/**
* @fn	bool inCone(const dvec3& spotPos, const dvec3& spotDir, double spotFOV, const dvec3& intercept)
* @brief	Determines if an intercept point falls within a spotlight's cone.
* @param	spotPos		where the spotlight is positioned
* @param	spotDir		normalized direction of spotlight's pointing direction
* @param	spotFOV		spotlight's field of view, which is 2X of the angle from the viewing axis
* @param	intercept	the position of the intercept.
*/

bool inCone(const dvec3& spotPos, const dvec3& spotDir, double spotFOV, const dvec3& intercept) {
	//if (std::acos(glm::dot(glm::normalize(spotDir), glm::normalize(intercept - spotPos))) <= spotFOV/2)
	return glm::dot(glm::normalize(spotDir), glm::normalize(intercept - spotPos)) > std::cos(spotFOV / 2);
}

/**
* @fn	bool inShadow(const dvec3& lightPos, const dvec3& intercept, const dvec3& normal, const vector<VisibleIShapePtr> objects)
* @brief	Determines if an intercept point falls in a shadow.
* @param	lightPos		where the spotlight is positioned //(positional light?)
* @param	intercept	the position of the intercept.
* @param	normal		the normal vector at the intercept point
* @param	objects		the collection of opaque objects in the scene
*/

bool inShadow(const dvec3& lightPos, const dvec3& intercept, const dvec3& normal, const vector<VisibleIShapePtr>& objects) {

	HitRecord objectHit;
	dvec3 raisedPt = IShape::movePointOffSurface(intercept, normal);
	Ray feeler = Ray(raisedPt, glm::normalize(lightPos - raisedPt));

	VisibleIShape::findIntersection(feeler, objects, objectHit);

	if (objectHit.t < glm::distance(raisedPt, lightPos))
	{
		return true;
	}

	return false;
}