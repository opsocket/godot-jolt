#pragma once

#include "joints/jolt_joint_impl_3d.hpp"

class JoltBodyImpl3D;
class JoltSpace3D;

class JoltPinJointImpl3D final : public JoltJointImpl3D {
public:
	JoltPinJointImpl3D(
		JoltBodyImpl3D* p_body_a,
		JoltBodyImpl3D* p_body_b,
		const Vector3& p_local_a,
		const Vector3& p_local_b,
		bool p_lock = true
	);

	JoltPinJointImpl3D(
		JoltBodyImpl3D* p_body_a,
		const Vector3& p_local_a,
		const Vector3& p_local_b,
		bool p_lock = true
	);

	PhysicsServer3D::JointType get_type() const override { return PhysicsServer3D::JOINT_TYPE_PIN; }

	Vector3 get_local_a() const { return local_a; }

	void set_local_a(const Vector3& p_local_a, bool p_lock = true);

	Vector3 get_local_b() const { return local_b; }

	void set_local_b(const Vector3& p_local_b, bool p_lock = true);

	double get_param(PhysicsServer3D::PinJointParam p_param) const;

	void set_param(PhysicsServer3D::PinJointParam p_param, double p_value);

	void rebuild(bool p_lock = true) override;

private:
	Vector3 local_a;

	Vector3 local_b;
};
