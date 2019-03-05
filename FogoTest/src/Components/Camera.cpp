#include "./Camera.h"

using namespace DirectX;
using Fogo::Utility::Window;

Camera::Degree::Degree(float initValue) : raw(initValue) {

}

Camera::Degree::Degree() : Degree(0) {

}

float Camera::Degree::toRadian() const {
	return DirectX::XMConvertToRadians(raw);
}

Camera::Degree::operator float() const {
	return raw;
}

Camera::Degree & Camera::Degree::operator=(const float newValue) {
	raw = newValue;
	return * this;
}

XMMATRIX Camera::getView() const {
	return XMMatrixLookAtLH (
		XMVECTOR { position.x, position.y, position.z },
		XMVECTOR { target.x, target.y, target.z },
		XMVECTOR { up.x, up.y, up.z }
	);
}

XMMATRIX Camera::getProjection() const {
	return XMMatrixPerspectiveFovLH(
		fov.toRadian(),
		static_cast<float>(Window::GetWidth()) / static_cast<float>(Window::GetHeight()),
		nearZ,
		farZ
	);
}