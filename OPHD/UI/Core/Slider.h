/*!
 * \file	Slider.h
 * \brief	Header for sliding controls.
 * \author Goof
 */
#pragma once
#include "Button.h"
#include "Control.h"
#include "../../Common.h"

#include "NAS2D/Timer.h"

 /**
 * \class	Slider
 * \brief	Base class for all sliding controls.
 * 
 * Handle horizontal and vertical slide bar.
 * The handle could go backward or forward.
 * the handle width/height depend on the max value.
 * the minimum width/height is the height/width of the slide bar
 */
class Slider : public Control
{
public:
	/*!
	 * \enum 	SliderType
	 * \brief	List the types of slider that could be used
	 */
	enum SliderType
	{
		SLIDER_VERTICAL, /*!< Vertical slider. */
		SLIDER_HORIZONTAL /*!< Horizontal slider. */
	};

	using ValueChangedCallback = NAS2D::Signals::Signal<float>; /*!< type for Callback on value changed. */

public:
	Slider();
	~Slider() override;

	void thumbPosition(float value); /*!< Set the current position. */
	float thumbPosition(); /*!< Get the current position. */
	void changeThumbPosition(float change); /*!< Adds the change amount to the current position. */

	void thumbPositionNormalized(float value); /*!< Set the current position. */
	float thumbPositionNormalized(); /*!< Get the current position. */

	bool displayPosition() { return mDisplayPosition;} /*!< Get the position display flag. */
	void displayPosition(bool value) { mDisplayPosition = value; } /*!< Set the position display flag. */

	float length(); /*!< Get the max value for the slide area. */
	void length(float length); /*!< Set the max value for the slide area. */
	
	bool backward() { return mBackward; } /*!< Get the backward flag. */
	void backward(bool isBackward) { mBackward = isBackward; } /*!< Set the backward flag. */

	void update() override; /*!< Called to display the slider. */
	void size(NAS2D::Vector<float> size); /*!< Set the slider size. */

	ValueChangedCallback& change() { return mCallback; } /*!< Give the callback to enable another control or a window to dis/connect to this event call. */

protected:
	virtual void onMouseDown(NAS2D::EventHandler::MouseButton button, int x, int y); /*!< Event raised on mouse button down. */
	virtual void onMouseUp(NAS2D::EventHandler::MouseButton button, int x, int y); /*!< Event raised on mouse button up. */
	virtual void onMouseMove(int x, int y, int dX, int dY); /*!< Event raised on mouse move. */
	
private:
	float positionInternal();
	void positionInternal(float newPosition);

	void setSkins(); /*!< Helper function that load the gui skin on the first update call. */
	void draw() override; /*!< Draw the widget on screen. */
	void logic(); /*!< Compute some values before drawing the control. */

	void _buttonCheck(bool& buttonFlag, NAS2D::Rectangle<float>& rect, float value);

private:
	NAS2D::Timer mTimer;

	ValueChangedCallback mCallback; /*!< Callback executed when the value is changed. */

	SliderType mSliderType{SliderType::SLIDER_VERTICAL}; /*!< Type of the Slider. */
	
	// mouse event related vars
	NAS2D::Point<int> mMousePosition; /**< Mouse coordinates. */

	bool mMouseHoverSlide = false; /*!< Mouse is within the bounds of the Button. */
	bool mThumbPressed = false; /*!< Flag to indicate if this control is pressed. */

	// Slider values
	float mPosition = 0.0f; /*!< Current value that represent the position of the slider. */
	float mLength = 0.0f; /*!< Maximum value for the position of the slider. */

	bool mBackward = false; /*!< Does the value returned in backward mode . */

	// Slider button responses
	uint32_t mPressedAccumulator = 0; /**< Accumulation value for pressed responses. */
	bool mButton1Held = false; /**< Flag indicating that a button is being held down. */
	bool mButton2Held = false; /**< Flag indicating that a button is being held down. */


	// drawing vars
	NAS2D::ImageList mSkinButton1; /*!< Skin for button 1 (Up or Left). */
	NAS2D::ImageList mSkinButton2; /*!< Skin for button 2 (Down or Right). */
	NAS2D::ImageList mSkinMiddle; /*!< Skin for the slide area. */
	
	NAS2D::ImageList mSkinSlider; /*!< Skin for the slider. */
	bool mDisplayPosition = false; /*!< Indicate if the slider display the value on mouse over. */
	
	NAS2D::Rectangle<float> mButton1; /*!< Area on screen where the second button is displayed. (Down/Left) */
	NAS2D::Rectangle<float> mButton2; /*!< Area on screen where the first button is displayed. (Up/Right)*/
	NAS2D::Rectangle<float> mSlideBar; /*!< Area on screen where the slide area is displayed. */
	NAS2D::Rectangle<float> mSlider; /*!< Area on screen where the slider is displayed. */
};