#include "jolt_pin_joint_impl_3d.hpp"

#include "objects/jolt_body_impl_3d.hpp"
#include "spaces/jolt_space_3d.hpp"

namespace {

constexpr double DEFAULT_BIAS = 0.3;
constexpr double DEFAULT_DAMPING = 1.0;
constexpr double DEFAULT_IMPULSE_CLAMP = 0.0;

} // namespace

JoltPinJointImpl3D::JoltPinJointImpl3D(
	JoltBodyImpl3D* p_body_a,
	JoltBodyImpl3D* p_body_b,
	const Vector3& p_local_a,
	const Vector3& p_local_b,
	bool p_lock
)
	: JoltJointImpl3D(p_body_a, p_body_b, Transform3D({}, p_local_a), Transform3D({}, p_local_b))
	, local_a(p_local_a)
	, local_b(p_local_b) {
	rebuild(p_lock);
}

JoltPinJointImpl3D::JoltPinJointImpl3D(
	JoltBodyImpl3D* p_body_a,
	const Vector3& p_local_a,
	const Vector3& p_local_b,
	bool p_lock
)
	: JoltJointImpl3D(p_body_a, Transform3D({}, p_local_b))
	, local_a(p_local_a)
	, local_b(p_local_b) {
	rebuild(p_lock);
}

void JoltPinJointImpl3D::set_local_a(const Vector3& p_local_a, bool p_lock) {
	local_a = p_local_a;

	world_ref = body_a->get_transform_scaled() * Transform3D({}, local_a);

	rebuild(p_lock);
}

void JoltPinJointImpl3D::set_local_b(const Vector3& p_local_b, bool p_lock) {
	local_b = p_local_b;

	if (body_b != nullptr) {
		world_ref = body_b->get_transform_scaled() * Transform3D({}, local_b);
	} else {
		world_ref = Transform3D({}, local_b);
	}

	rebuild(p_lock);
}

double JoltPinJointImpl3D::get_param(PhysicsServer3D::PinJointParam p_param) const {
	switch (p_param) {
		case PhysicsServer3D::PIN_JOINT_BIAS: {
			return DEFAULT_BIAS;
		}
		case PhysicsServer3D::PIN_JOINT_DAMPING: {
			return DEFAULT_DAMPING;
		}
		case PhysicsServer3D::PIN_JOINT_IMPULSE_CLAMP: {
			return DEFAULT_IMPULSE_CLAMP;
		}
		default: {
			ERR_FAIL_D_MSG(vformat("Unhandled pin joint parameter: '%d'", p_param));
		}
	}
}

void JoltPinJointImpl3D::set_param(PhysicsServer3D::PinJointParam p_param, double p_value) {
	switch (p_param) {
		case PhysicsServer3D::PIN_JOINT_BIAS: {
			if (!Math::is_equal_approx(p_value, DEFAULT_BIAS)) {
				WARN_PRINT(vformat(
					"Pin joint bias is not supported by Godot Jolt. "
					"Any such value will be ignored. "
					"This joint connects %s.",
					owners_to_string()
				));
			}
		} break;
		case PhysicsServer3D::PIN_JOINT_DAMPING: {
			if (!Math::is_equal_approx(p_value, DEFAULT_DAMPING)) {
				WARN_PRINT(vformat(
					"Pin joint damping is not supported by Godot Jolt. "
					"Any such value will be ignored. "
					"This joint connects %s.",
					owners_to_string()
				));
			}
		} break;
		case PhysicsServer3D::PIN_JOINT_IMPULSE_CLAMP: {
			if (!Math::is_equal_approx(p_value, DEFAULT_IMPULSE_CLAMP)) {
				WARN_PRINT(vformat(
					"Pin joint impulse clamp is not supported by Godot Jolt. "
					"Any such value will be ignored. "
					"This joint connects %s.",
					owners_to_string()
				));
			}
		} break;
		default: {
			ERR_FAIL_MSG(vformat("Unhandled pin joint parameter: '%d'", p_param));
		} break;
	}
}

void JoltPinJointImpl3D::rebuild(bool p_lock) {
	destroy();

	JoltSpace3D* space = get_space();

	if (space == nullptr) {
		return;
	}

	JPH::BodyID body_ids[2] = {body_a->get_jolt_id()};
	int32_t body_count = 1;

	if (body_b != nullptr) {
		body_ids[1] = body_b->get_jolt_id();
		body_count = 2;
	}

	const JoltWritableBodies3D bodies = space->write_bodies(body_ids, body_count, p_lock);

	JPH::PointConstraintSettings constraint_settings;
	constraint_settings.mSpace = JPH::EConstraintSpace::WorldSpace;
	constraint_settings.mPoint1 = to_jolt(world_ref.origin);
	constraint_settings.mPoint2 = to_jolt(world_ref.origin);

	if (body_b != nullptr) {
		const JoltWritableBody3D jolt_body_a = bodies[0];
		ERR_FAIL_COND(jolt_body_a.is_invalid());

		const JoltWritableBody3D jolt_body_b = bodies[1];
		ERR_FAIL_COND(jolt_body_b.is_invalid());

		jolt_ref = constraint_settings.Create(*jolt_body_a, *jolt_body_b);
	} else {
		const JoltWritableBody3D jolt_body_a = bodies[0];
		ERR_FAIL_COND(jolt_body_a.is_invalid());

		jolt_ref = constraint_settings.Create(*jolt_body_a, JPH::Body::sFixedToWorld);
	}

	space->add_joint(this);
}
