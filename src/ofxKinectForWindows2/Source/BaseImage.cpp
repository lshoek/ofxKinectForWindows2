#include "BaseImage.h"
#include "ofMain.h"

#define CHECK_OPEN if(!this->reader) { OFXKINECTFORWINDOWS2_ERROR << "Failed : Reader is not open"; }

namespace ofxKinectForWindows2 {
	namespace Source {
#pragma mark BaseImage
		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::BaseImage() {
			this->reader = NULL;
			this->useTexture = true;
			this->diagonalFieldOfView = 0.0f;
			this->horizontalFieldOfView = 0.0f;
			this->verticalFieldOfView = 0.0f;
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::~BaseImage() {
			SafeRelease(this->reader);
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		ReaderType * BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getReader() {
			return this->reader;
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		ofTexture & BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getTextureReference() {
			return this->texture;
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		void BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::setUseTexture(bool useTexture) {
			this->useTexture = useTexture;
			if (!useTexture) {
				this->texture.clear();
			}
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		PixelType * BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getPixels() {
			return this->pixels.getPixels();
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		ofPixels_<PixelType> & BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getPixelsRef() {
			return this->pixels;
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		void BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::draw(float x, float y) {
			this->texture.draw(x, y);
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		void BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::draw(float x, float y, float width, float height) {
			this->texture.draw(x, y, width, height);
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		float BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getWidth() {
			return this->pixels.getWidth();
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		float BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getHeight() {
			return this->pixels.getHeight();
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		float BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getDiagonalFieldOfView() const {
			return this->diagonalFieldOfView;
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		float BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getHorizontalFieldOfView() const {
			return this->horizontalFieldOfView;
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		float BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getVerticalFieldOfView() const {
			return this->verticalFieldOfView;
		}

#pragma mark BaseImageSimple
		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		void BaseImageSimple OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::update() {
			CHECK_OPEN

			FrameType * frame = NULL;
			IFrameDescription * frameDescription = NULL;
			try {
				//acquire frame
				if (FAILED(this->reader->AcquireLatestFrame(&frame))) {
					return; // we often throw here when no new frame is available
				}

				//allocate pixels and texture if we need to
				if (FAILED(frame->get_FrameDescription(&frameDescription))) {
					throw Exception("Failed to get frame description");
				}
				int width, height;
				if (FAILED(frameDescription->get_Width(&width)) || FAILED(frameDescription->get_Height(&height))) {
					throw Exception("Failed to get width and height of frame");
				}
				if (width != this->pixels.getWidth() || height != this->texture.getHeight()) {
					this->pixels.allocate(width, height, OF_IMAGE_GRAYSCALE);
					this->texture.allocate(this->pixels);
				}

				//update local assets
				if (FAILED(frame->CopyFrameDataToArray(this->pixels.size(), this->pixels.getPixels()))) {
					throw Exception("Couldn't pull pixel buffer ");
				}
				if (this->useTexture) {
					this->texture.loadData(this->pixels);
				}

				//update field of view
				if (FAILED(frameDescription->get_DiagonalFieldOfView(&this->diagonalFieldOfView))) {
					throw Exception("Failed to get diagonal field of view");
				}
				if (FAILED(frameDescription->get_DiagonalFieldOfView(&this->diagonalFieldOfView))) {
					throw Exception("Failed to get diagonal field of view");
				}
				if (FAILED(frameDescription->get_DiagonalFieldOfView(&this->diagonalFieldOfView))) {
					throw Exception("Failed to get diagonal field of view");
				}
			} catch (std::exception & e) {
				OFXKINECTFORWINDOWS2_ERROR << e.what();
			}
			SafeRelease(frameDescription);
			SafeRelease(frame);
		}

		//---------
		template class BaseImageSimple<unsigned short, IDepthFrameReader, IDepthFrame>;
		template class BaseImageSimple<unsigned short, IInfraredFrameReader, IInfraredFrame>;
		template class BaseImageSimple<unsigned short, ILongExposureInfraredFrameReader, ILongExposureInfraredFrame>;
		template class BaseImageSimple<unsigned char, IBodyIndexFrameReader, IBodyIndexFrame>;
		template class BaseImage<unsigned short, IDepthFrameReader, IDepthFrame>;
		template class BaseImage<unsigned short, IInfraredFrameReader, IInfraredFrame>;
		template class BaseImage<unsigned short, ILongExposureInfraredFrameReader, ILongExposureInfraredFrame>;
		template class BaseImage<unsigned char, IBodyIndexFrameReader, IBodyIndexFrame>;
		template class BaseImage<unsigned char, IColorFrameReader, IColorFrame>;
	}
}