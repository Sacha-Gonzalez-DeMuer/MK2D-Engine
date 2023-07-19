#pragma once

namespace dae
{
	class ICollider;
	class IObject
	{
	public:
		virtual void Start() {};
		virtual void Update() {};
		virtual void Render() const {};
		virtual void OnCollisionEnter(ICollider& /*other*/) {};
		virtual void OnCollisionExit(ICollider& /*other*/) {};
	};
}