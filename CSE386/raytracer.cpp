/****************************************************
 * 2016-2021 Eric Bachmann and Mike Zmuda
 * All Rights Reserved.
 * NOTICE:
 * Dissemination of this information or reproduction
 * of this material is prohibited unless prior written
 * permission is granted.
 ****************************************************/
#include "raytracer.h"
#include "ishape.h"
#include "io.h"

/**
 * @fn	RayTracer::RayTracer(const color &defa)
 * @brief	Constructs a raytracers.
 * @param	defa	The clear color.
 */

RayTracer::RayTracer(const color &defa)
	: defaultColor(defa) {
}

/**
 * @fn	void RayTracer::raytraceScene(FrameBuffer &frameBuffer, int depth, const IScene &theScene) const
 * @brief	Raytrace scene
 * @param [in,out]	frameBuffer	Framebuffer.
 * @param 		  	depth	   	The current depth of recursion.
 * @param 		  	theScene   	The scene.
 * @param 		  	N   	    Number of rays per pixel for anti-aliasing.
 * @param 		  	viewStart   The x and y of the lower left pixel of the viewport.
 * @param 		  	viewEnd   	The x and y of the top right pixel of the viewport.
 */

void RayTracer::raytraceScene(FrameBuffer &frameBuffer, int depth,
								const IScene &theScene, int N, const dvec2& viewStart, const dvec2& viewEnd) const {
	const RaytracingCamera &camera = *theScene.camera;
	const vector<VisibleIShapePtr> &opaqueObjs = theScene.opaqueObjs;
	const vector<VisibleIShapePtr> &transObjs = theScene.transparentObjs;
	const vector<PositionalLightPtr> &lights = theScene.lights;

	for (int y = viewStart.y; y < viewEnd.y; ++y) {
		for (int x = viewStart.x; x < viewEnd.x; ++x) {
			color sum = black;
			int newX = map(x, viewStart.x, viewEnd.x, 0, camera.getNX());
			int newY = map(y, viewStart.y, viewEnd.y, 0, camera.getNY());
			Ray ray = camera.getRay(newX, newY);

			for (int r = 0; r < N; r++)
			{
				for (int c = 0; c < N; c++)
				{
					DEBUG_PIXEL = (x == xDebug && y == yDebug);
					if (DEBUG_PIXEL) {
						cout << "";
					}

					int newX = map(x, viewStart.x, viewEnd.x, 0, camera.getNX());
					int newY = map(y, viewStart.y, viewEnd.y, 0, camera.getNY());
					Ray ray = camera.getRay(newX + 1 / (2.0 * N) + r * 1.0 / N, 
						newY + 1 / (2.0 * N) + c * 1.0 / N);

					color pixelColor = defaultColor;

					HitRecord opaqueHit;
					HitRecord transHit;

					VisibleIShape::findIntersection(ray, opaqueObjs, opaqueHit);
					VisibleIShape::findIntersection(ray, transObjs, transHit);

					color opaqueColor = traceIndividualRay(ray, theScene, depth + 1);

					color transColor;
					if (transHit.t != FLT_MAX)
					{
						for (int i = 0; i < lights.size(); i++)
						{
							transColor += lights[i]->illuminate(transHit.interceptPt, transHit.normal, transHit.material,
								(*theScene.camera).getFrame(), true);
						}
					}

					// intersects both opaque and translucent object
					if (opaqueHit.t != FLT_MAX && transHit.t != FLT_MAX)
					{
						// checks if opaque object is in front of translucent
						if (opaqueHit.t < transHit.t)
						{
							pixelColor = opaqueColor;
						}
						else
						{
							pixelColor = (1 - transHit.material.alpha) * opaqueColor + 
								transHit.material.alpha * transColor;
						}
					}
					// intersects opaque object only
					else if (opaqueHit.t != FLT_MAX)
					{
						pixelColor = opaqueColor;
					}
					// intersects translucent object only
					else if (transHit.t != FLT_MAX)
					{
						pixelColor = (1 - transHit.material.alpha) * defaultColor + transHit.material.alpha * transColor;
					}

					sum += glm::clamp(pixelColor, 0.0, 1.0);
				}
			}

			frameBuffer.setColor(x, y, sum/glm::pow(N, 2.0));

			frameBuffer.showAxes(x, y, ray, 0.05);			// Displays R/x, G/y, B/z axes
		}
	}

	frameBuffer.showColorBuffer();
}

/**
 * @fn	color RayTracer::traceIndividualRay(const Ray &ray, 
 *											const IScene &theScene,
 *											int recursionLevel) const
 * @brief	Trace an individual ray.
 * @param	ray			  	The ray.
 * @param	theScene	  	The scene.
 * @param	recursionLevel	The recursion level.
 * @return	The color to be displayed as a result of this ray.
 */

color RayTracer::traceIndividualRay(const Ray &ray, const IScene &theScene, int recursionLevel) const {
	HitRecord opaqueHit;

	VisibleIShape::findIntersection(ray, theScene.opaqueObjs, opaqueHit);

	color opaqueColor = black;
	if (opaqueHit.t != FLT_MAX)
	{
		for (int i = 0; i < theScene.lights.size(); i++)
		{
			opaqueColor += theScene.lights[i]->illuminate(opaqueHit.interceptPt, opaqueHit.normal, opaqueHit.material,
				(*theScene.camera).getFrame(), inShadow(theScene.lights[i]->actualPosition((*theScene.camera).getFrame()),
					opaqueHit.interceptPt, opaqueHit.normal, theScene.opaqueObjs));
		}

		if (opaqueHit.texture != nullptr)
		{
			color texel = opaqueHit.texture->getPixelUV(opaqueHit.u, opaqueHit.v);
			opaqueColor = 0.5 * texel + 0.5 * opaqueColor;
		}

		if (recursionLevel - 1 > 0)
		{
			color newOrigin = IShape::movePointOffSurface(opaqueHit.interceptPt, opaqueHit.normal);
			color newDirection = ray.dir - 2 * (glm::dot(ray.dir, opaqueHit.normal)) * opaqueHit.normal;
			opaqueColor += 0.3 * traceIndividualRay(Ray(newOrigin, newDirection), theScene, recursionLevel - 1);
		}
	}
	
	return opaqueColor;
}