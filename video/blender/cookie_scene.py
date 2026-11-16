"""Photorealistic cookie still life rendered with Blender Cycles.

Usage (bpy module or `blender -b -P cookie_scene.py -- ...`):
    python cookie_scene.py --out DIR --frames 1,97,193      # stills
    python cookie_scene.py --out DIR --range 1 288          # animation frames
Options: --samples N (default 64), --scale PCT (default 100)
"""
import argparse
import math
import os
import random
import sys

import bpy  # must come first: it registers bmesh and mathutils
import bmesh
from mathutils import Vector

FPS = 24
SHOT = 96  # frames per shot
TOTAL = SHOT * 3
R = 0.04  # cookie radius in metres
BITES = ((0.55, 0.34, 1.02), (0.95, 0.30, 1.0), (0.2, 0.26, 1.05))  # angle, radius, distance


def parse():
    argv = sys.argv[sys.argv.index("--") + 1:] if "--" in sys.argv else sys.argv[1:]
    p = argparse.ArgumentParser()
    p.add_argument("--out", required=True)
    p.add_argument("--frames", default="")
    p.add_argument("--range", nargs=2, type=int)
    p.add_argument("--samples", type=int, default=64)
    p.add_argument("--scale", type=int, default=100)
    return p.parse_args(argv)


# ---------- materials ----------

def new_mat(name):
    m = bpy.data.materials.new(name)
    m.use_nodes = True
    nt = m.node_tree
    bsdf = nt.nodes["Principled BSDF"]
    return m, nt, bsdf


def node(nt, kind, **props):
    n = nt.nodes.new(kind)
    for k, v in props.items():
        setattr(n, k, v)
    return n


def ramp(nt, stops):
    n = nt.nodes.new("ShaderNodeValToRGB")
    els = n.color_ramp.elements
    while len(els) < len(stops):
        els.new(0.5)
    for el, (pos, col) in zip(els, stops):
        el.position = pos
        el.color = (*col, 1)
    return n


def cookie_material():
    m, nt, b = new_mat("Cookie")
    L = nt.links.new
    tc = node(nt, "ShaderNodeTexCoord")
    # patchy bake colour
    n1 = node(nt, "ShaderNodeTexNoise")
    n1.inputs["Scale"].default_value = 3.5
    n1.inputs["Detail"].default_value = 6
    L(tc.outputs["Object"], n1.inputs["Vector"])
    col = ramp(nt, [(0.3, (0.36, 0.17, 0.05)), (0.55, (0.62, 0.36, 0.13)), (0.8, (0.78, 0.52, 0.23))])
    L(n1.outputs["Fac"], col.inputs["Fac"])
    # darker towards the rim
    sep = node(nt, "ShaderNodeSeparateXYZ")
    L(tc.outputs["Object"], sep.inputs[0])
    comb = node(nt, "ShaderNodeCombineXYZ")
    L(sep.outputs["X"], comb.inputs["X"])
    L(sep.outputs["Y"], comb.inputs["Y"])
    ln = node(nt, "ShaderNodeVectorMath", operation="LENGTH")
    L(comb.outputs[0], ln.inputs[0])
    rim = ramp(nt, [(0.6, (1, 1, 1)), (1.0, (0.5, 0.36, 0.25))])
    L(ln.outputs["Value"], rim.inputs["Fac"])
    mul = node(nt, "ShaderNodeMixRGB", blend_type="MULTIPLY")
    mul.inputs["Fac"].default_value = 1
    L(col.outputs["Color"], mul.inputs["Color1"])
    L(rim.outputs["Color"], mul.inputs["Color2"])
    L(mul.outputs["Color"], b.inputs["Base Color"])
    b.inputs["Roughness"].default_value = 0.78
    b.inputs["Subsurface Weight"].default_value = 0.12
    b.inputs["Subsurface Radius"].default_value = (0.012, 0.006, 0.003)
    b.inputs["Subsurface Scale"].default_value = 0.2
    # crumbly surface: fine grain + cracks
    fine = node(nt, "ShaderNodeTexNoise")
    fine.inputs["Scale"].default_value = 45
    fine.inputs["Detail"].default_value = 8
    L(tc.outputs["Object"], fine.inputs["Vector"])
    vor = node(nt, "ShaderNodeTexVoronoi", feature="DISTANCE_TO_EDGE")
    vor.inputs["Scale"].default_value = 4.5
    warp = node(nt, "ShaderNodeTexNoise")
    warp.inputs["Scale"].default_value = 4
    wmix = node(nt, "ShaderNodeMixRGB")
    wmix.inputs["Fac"].default_value = 0.45
    L(tc.outputs["Object"], wmix.inputs["Color1"])
    L(warp.outputs["Color"], wmix.inputs["Color2"])
    L(wmix.outputs["Color"], vor.inputs["Vector"])
    crack = ramp(nt, [(0.0, (0, 0, 0)), (0.06, (1, 1, 1))])
    L(vor.outputs["Distance"], crack.inputs["Fac"])
    b1 = node(nt, "ShaderNodeBump")
    b1.inputs["Strength"].default_value = 0.35
    b1.inputs["Distance"].default_value = 0.02
    L(fine.outputs["Fac"], b1.inputs["Height"])
    b2 = node(nt, "ShaderNodeBump")
    b2.inputs["Strength"].default_value = 0.22
    b2.inputs["Distance"].default_value = 0.03
    L(crack.outputs["Color"], b2.inputs["Height"])
    L(b1.outputs["Normal"], b2.inputs["Normal"])
    L(b2.outputs["Normal"], b.inputs["Normal"])
    return m


def chocolate_material():
    m, nt, b = new_mat("Chocolate")
    b.inputs["Base Color"].default_value = (0.07, 0.032, 0.016, 1)
    b.inputs["Roughness"].default_value = 0.32
    b.inputs["Coat Weight"].default_value = 0.25
    b.inputs["Coat Roughness"].default_value = 0.2
    n = node(nt, "ShaderNodeTexNoise")
    n.inputs["Scale"].default_value = 12
    bump = node(nt, "ShaderNodeBump")
    bump.inputs["Strength"].default_value = 0.15
    nt.links.new(n.outputs["Fac"], bump.inputs["Height"])
    nt.links.new(bump.outputs["Normal"], b.inputs["Normal"])
    return m


def wood_material():
    m, nt, b = new_mat("Wood")
    L = nt.links.new
    tc = node(nt, "ShaderNodeTexCoord")
    mp = node(nt, "ShaderNodeMapping")
    mp.inputs["Scale"].default_value = (6, 0.35, 1)
    L(tc.outputs["Object"], mp.inputs["Vector"])
    w = node(nt, "ShaderNodeTexNoise")
    w.inputs["Scale"].default_value = 22
    w.inputs["Detail"].default_value = 5
    w.inputs["Roughness"].default_value = 0.5
    w.inputs["Distortion"].default_value = 0.15
    L(mp.outputs["Vector"], w.inputs["Vector"])
    col = ramp(nt, [(0.28, (0.09, 0.04, 0.018)), (0.5, (0.24, 0.12, 0.05)), (0.72, (0.38, 0.21, 0.1))])
    L(w.outputs["Fac"], col.inputs["Fac"])
    L(col.outputs["Color"], b.inputs["Base Color"])
    rr = node(nt, "ShaderNodeMapRange")
    rr.inputs["To Min"].default_value = 0.32
    rr.inputs["To Max"].default_value = 0.55
    L(w.outputs["Fac"], rr.inputs["Value"])
    L(rr.outputs["Result"], b.inputs["Roughness"])
    bump = node(nt, "ShaderNodeBump")
    bump.inputs["Strength"].default_value = 0.12
    bump.inputs["Distance"].default_value = 0.002
    L(w.outputs["Fac"], bump.inputs["Height"])
    L(bump.outputs["Normal"], b.inputs["Normal"])
    return m


def ceramic_material():
    m, nt, b = new_mat("Ceramic")
    n = node(nt, "ShaderNodeTexNoise")
    n.inputs["Scale"].default_value = 900
    c = ramp(nt, [(0.62, (0.80, 0.79, 0.75)), (0.75, (0.42, 0.38, 0.33))])
    nt.links.new(n.outputs["Fac"], c.inputs["Fac"])
    nt.links.new(c.outputs["Color"], b.inputs["Base Color"])
    b.inputs["Roughness"].default_value = 0.22
    b.inputs["Coat Weight"].default_value = 0.6
    b.inputs["Coat Roughness"].default_value = 0.08
    return m


def simple_material(name, color, rough, **extra):
    m, nt, b = new_mat(name)
    b.inputs["Base Color"].default_value = (*color, 1)
    b.inputs["Roughness"].default_value = rough
    for k, v in extra.items():
        b.inputs[k].default_value = v
    return m


# ---------- geometry ----------

def link(obj):
    bpy.context.scene.collection.objects.link(obj)
    return obj


def smooth(obj):
    for p in obj.data.polygons:
        p.use_smooth = True


def clouds(name, size, seed_depth=2):
    t = bpy.data.textures.new(name, "CLOUDS")
    t.noise_scale = size
    t.noise_depth = seed_depth
    return t


def cookie_top(r):
    """Height of the cookie's upper surface at radius r (unit cookie)."""
    r = min(abs(r), 1.0)
    return 0.24 * math.sqrt(max(0.0, 1 - r ** 4)) ** 0.8


def make_cookie(name, mat, rng, bite=False):
    bm = bmesh.new()
    bmesh.ops.create_uvsphere(bm, u_segments=96, v_segments=48, radius=1)
    for v in bm.verts:
        r = math.hypot(v.co.x, v.co.y)
        if v.co.z >= 0:
            v.co.z = cookie_top(r)
        else:
            v.co.z = -0.05 * (1 - min(r, 1) ** 6)
    if bite:
        for _ in range(3):
            for ang, rad, dist in BITES:
                c = Vector((math.cos(ang) * dist, math.sin(ang) * dist))
                for v in bm.verts:
                    d = Vector((v.co.x, v.co.y)) - c
                    if d.length < rad:
                        # push to the bite's edge, biased towards the cookie centre so the rim never flares out
                        d = (d + (-c.normalized()) * rad * 0.7).normalized() * rad
                        q = c + d
                        if q.length > 1.0:
                            q = q.normalized()
                        v.co.x, v.co.y = q.x, q.y
    me = bpy.data.meshes.new(name)
    bm.to_mesh(me)
    bm.free()
    ob = link(bpy.data.objects.new(name, me))
    smooth(ob)
    me.materials.append(mat)
    ob.modifiers.new("sub", "SUBSURF").levels = 1
    ob.modifiers["sub"].render_levels = 2
    off = Vector((rng.uniform(-5, 5), rng.uniform(-5, 5), rng.uniform(-5, 5)))
    for nm, size, strength in (("lump", 0.45, 0.10), ("mid", 0.16, 0.035), ("grain", 0.04, 0.012)):
        d = ob.modifiers.new(nm, "DISPLACE")
        d.texture = clouds(f"{name}_{nm}", size)
        d.strength = strength
        d.mid_level = 0.5
        d.texture_coords = "OBJECT"
        e = bpy.data.objects.new(f"{name}_{nm}_e", None)
        link(e)
        e.parent = ob
        e.location = off
        e.hide_render = True
        d.texture_coords_object = e
    return ob


def add_chips(cookie, mat, rng, n=11, bite=False, avoid=()):
    for i in range(n):
        for _ in range(40):
            ang = rng.uniform(0, math.tau)
            r = math.sqrt(rng.uniform(0.0, 0.72))
            x, y = math.cos(ang) * r, math.sin(ang) * r
            if bite and any(math.hypot(x - math.cos(a) * d, y - math.sin(a) * d) < rad + 0.12
                            for a, rad, d in BITES):
                continue
            if any(math.hypot(x - ax, y - ay) < ar for ax, ay, ar in avoid):
                continue
            break
        bm = bmesh.new()
        bmesh.ops.create_icosphere(bm, subdivisions=3, radius=1)
        for v in bm.verts:
            if v.co.z < -0.2:
                v.co.z = -0.2
        me = bpy.data.meshes.new(f"{cookie.name}_chip{i}")
        bm.to_mesh(me)
        bm.free()
        ch = link(bpy.data.objects.new(me.name, me))
        smooth(ch)
        me.materials.append(mat)
        s = rng.uniform(0.085, 0.14)
        ch.scale = (s, s * rng.uniform(0.75, 1.1), s * rng.uniform(0.4, 0.6))
        ch.location = (x, y, cookie_top(r) - 0.012)
        ch.rotation_euler = (rng.uniform(-0.3, 0.3), rng.uniform(-0.3, 0.3), rng.uniform(0, 6))
        d = ch.modifiers.new("d", "DISPLACE")
        d.texture = clouds(me.name + "_t", 0.6)
        d.strength = 0.35
        ch.parent = cookie


def lathe(name, profile, mat, thickness=0.0, steps=128):
    me = bpy.data.meshes.new(name)
    verts = [(x, 0, z) for x, z in profile]
    me.from_pydata(verts, [(i, i + 1) for i in range(len(verts) - 1)], [])
    ob = link(bpy.data.objects.new(name, me))
    sc = ob.modifiers.new("screw", "SCREW")
    sc.steps = sc.render_steps = steps
    sc.use_merge_vertices = True
    sc.use_smooth_shade = True
    if thickness:
        so = ob.modifiers.new("solid", "SOLIDIFY")
        so.thickness = thickness
        so.offset = 1
    ob.modifiers.new("sub", "SUBSURF").render_levels = 2
    me.materials.append(mat)
    return ob


def plane(name, size, mat, loc=(0, 0, 0), rot=(0, 0, 0)):
    bm = bmesh.new()
    bmesh.ops.create_grid(bm, x_segments=1, y_segments=1, size=size)
    me = bpy.data.meshes.new(name)
    bm.to_mesh(me)
    bm.free()
    ob = link(bpy.data.objects.new(name, me))
    ob.location, ob.rotation_euler = loc, rot
    me.materials.append(mat)
    return ob


# ---------- scene ----------

def key(obj, path, frame, value):
    setattr(obj, path, value)
    obj.keyframe_insert(path, frame=frame)


FACE_AVOID = ((-0.29, 0.2, 0.30), (0.29, 0.2, 0.30), (0.0, -0.24, 0.36))  # x, y, radius on the unit cookie


def build_character(m_cookie, m_choc, rng):
    """The hero: an upright bitten cookie with eyes, a smile, arms and legs. Returns (root, pose)."""
    Rr = R * 1.1
    leg = 0.024
    rest_z = leg + Rr * 0.9

    def empty(name, parent=None, loc=(0, 0, 0)):
        e = link(bpy.data.objects.new(name, None))
        e.parent, e.location = parent, loc
        return e

    def ball(name, mat, parent, loc, scale, seg=32):
        bm = bmesh.new()
        bmesh.ops.create_uvsphere(bm, u_segments=seg, v_segments=seg // 2, radius=1)
        me = bpy.data.meshes.new(name)
        bm.to_mesh(me)
        bm.free()
        o = link(bpy.data.objects.new(name, me))
        smooth(o)
        me.materials.append(mat)
        o.parent, o.location, o.scale = parent, loc, scale
        return o

    root = empty("KRoot")
    body = empty("KBody", root, (0, 0, rest_z))
    c = make_cookie("Kriksis", m_cookie, rng, bite=True)
    add_chips(c, m_choc, rng, n=9, bite=True, avoid=FACE_AVOID)
    c.parent = body
    c.scale = (Rr,) * 3
    c.rotation_euler = (math.pi / 2, 0, 0)  # the cookie's top becomes the face, looking along -Y
    c.location = (0, 0.095 * Rr, 0)

    def face_y(r):
        return (0.095 - cookie_top(r)) * Rr

    white = simple_material("EyeWhite", (0.93, 0.92, 0.9), 0.12, **{"Coat Weight": 1.0, "Coat Roughness": 0.03})
    pupil = simple_material("Pupil", (0.012, 0.008, 0.006), 0.15, **{"Coat Weight": 1.0, "Coat Roughness": 0.02})
    glint = simple_material("Glint", (1, 1, 1), 0.3, **{"Emission Color": (1, 1, 1, 1), "Emission Strength": 2.0})
    eyes = []
    for sx in (-1, 1):
        ex, ez, er = sx * 0.29 * Rr, 0.2 * Rr, 0.2 * Rr
        g = empty(f"Eye{sx}", body, (ex, face_y(0.35) + 0.02 * Rr, ez))
        ball(f"EyeW{sx}", white, g, (0, 0, 0), (er, er * 0.55, er * 1.08))
        ball(f"EyeP{sx}", pupil, g, (-sx * 0.02 * Rr, -er * 0.42, -0.01 * Rr), (er * 0.52, er * 0.2, er * 0.56))
        ball(f"EyeG{sx}", glint, g, (-0.045 * Rr, -er * 0.6, 0.05 * Rr), (er * 0.13, er * 0.05, er * 0.13), seg=16)
        eyes.append(g)

    cu = bpy.data.curves.new("Mouth", "CURVE")
    cu.dimensions = "3D"
    sp = cu.splines.new("POLY")
    n = 24
    sp.points.add(n - 1)
    for i in range(n):
        a = math.radians(208 + (332 - 208) * i / (n - 1))
        x, z = math.cos(a) * 0.27 * Rr, -0.1 * Rr + math.sin(a) * 0.27 * Rr
        sp.points[i].co = (x, face_y(math.hypot(x, z) / Rr) - 0.01 * Rr, z, 1)
    cu.bevel_depth = 0.024 * Rr
    cu.bevel_resolution = 4
    cu.use_fill_caps = True
    cu.materials.append(simple_material("Mouth", (0.035, 0.012, 0.008), 0.45))
    mouth = link(bpy.data.objects.new("Mouth", cu))
    mouth.parent = body

    def limb(name, pivot_loc, length, radius, end_mat, end_scale, end_off=(0, 0, 0)):
        piv = empty(name, body, pivot_loc)
        bm = bmesh.new()
        bmesh.ops.create_cone(bm, cap_ends=True, segments=20, radius1=radius, radius2=radius, depth=length)
        for v in bm.verts:
            v.co.z -= length / 2
        me = bpy.data.meshes.new(name + "_m")
        bm.to_mesh(me)
        bm.free()
        o = link(bpy.data.objects.new(name + "_m", me))
        smooth(o)
        me.materials.append(m_cookie)
        o.parent = piv
        ball(name + "_end", end_mat, piv, (end_off[0], end_off[1], -length + end_off[2]), end_scale)
        return piv

    legs = [limb(f"Leg{sx}", (sx * 0.36 * Rr, 0.03 * Rr, -0.9 * Rr), leg - 0.005, 0.0026, m_choc,
                 (0.0065, 0.011, 0.005), (0, -0.004, 0)) for sx in (-1, 1)]
    arms = [limb(f"Arm{sx}", (sx * 0.96 * Rr, 0.03 * Rr, -0.08 * Rr), 0.027, 0.0024, m_cookie,
                 (0.0052, 0.0052, 0.0052)) for sx in (-1, 1)]

    A, B = Vector((-0.21, -0.03, 0)), Vector((0.0, -0.18, 0))
    heading = math.radians(53)

    def smoothstep(x):
        x = max(0.0, min(1.0, x))
        return x * x * (3 - 2 * x)

    def pose(f):
        # shot 1: walk in (1-84), turn to camera (84-96)
        u = smoothstep((f - 1) / 83) * 0.25 + min(1.0, (f - 1) / 83) * 0.75
        amp = 1.0 if f < 76 else max(0.0, (84 - f) / 8)
        ph = (f - 1) / 12 * math.tau
        swing = math.sin(ph) * amp
        bob = 0.0035 * abs(math.sin(ph)) * amp
        tilt = 0.07 * swing
        turn = heading * (1 - smoothstep((f - 84) / 12))
        arm_y = [math.radians(22), math.radians(-22)]  # left, right: hanging slightly outwards
        arm_x = [-0.5 * swing, 0.5 * swing]
        leg_x = [0.6 * swing, -0.6 * swing]
        jump, squash = 0.0, 1.0
        # shot 2: wave with the right arm
        if SHOT < f <= SHOT * 2:
            up = smoothstep((f - 116) / 12) - smoothstep((f - 172) / 12)
            wave = math.sin((f - 128) / 8 * math.tau) * math.radians(20) if 128 <= f <= 172 else 0.0
            arm_y[1] = math.radians(-22) + up * math.radians(-125) + wave * up
            tilt = 0.06 * up
            squash = 1 + 0.012 * math.sin(f / 30 * math.tau)
        # shot 3: two happy jumps with both arms up
        if f > SHOT * 2:
            for start in (200, 230):
                k = (f - start) / 22
                if 0 <= k <= 1:
                    jump = 0.032 * 4 * k * (1 - k)
                    leg_x = [0.35, -0.35]
                for edge in (start, start + 22):
                    d = abs(f - edge)
                    if d < 4:
                        squash = min(squash, 0.9 + 0.025 * d)
            up = smoothstep((f - 196) / 8) - smoothstep((f - 254) / 12)
            arm_y = [math.radians(22) + up * math.radians(125), math.radians(-22) - up * math.radians(125)]
        blink = 0.08 if any(b <= f < b + 3 for b in (40, 108, 164, 262)) else 1.0

        key(root, "location", f, tuple(A.lerp(B, u)))
        key(root, "rotation_euler", f, (0, 0, turn))
        key(body, "location", f, (0, 0, rest_z * squash + bob + jump))
        key(body, "rotation_euler", f, (0, tilt, 0))
        key(body, "scale", f, (2 - squash, 1, squash))
        for i in range(2):
            key(legs[i], "rotation_euler", f, (leg_x[i], 0, 0))
            key(arms[i], "rotation_euler", f, (arm_x[i], arm_y[i], 0))
            key(eyes[i], "scale", f, (1, 1, blink))

    return root, pose


def build():
    for o in list(bpy.data.objects):
        bpy.data.objects.remove(o, do_unlink=True)
    rng = random.Random(7)
    sc = bpy.context.scene
    m_cookie, m_choc = cookie_material(), chocolate_material()

    plane("Table", 1.5, wood_material())
    plane("Wall", 2.5, simple_material("Wall", (0.55, 0.5, 0.44), 0.9), loc=(0, 1.1, 1.0), rot=(math.pi / 2, 0, 0))

    lathe("Plate", [(0.0, 0.004), (0.078, 0.004), (0.098, 0.009), (0.118, 0.018)], ceramic_material(), thickness=0.004)
    plate_top = 0.0085

    def place(name, loc, rot=(0, 0, 0), bite=False, s=1.0):
        c = make_cookie(name, m_cookie, rng, bite=bite)
        add_chips(c, m_choc, rng, bite=bite)
        c.scale = (R * s,) * 3
        c.location, c.rotation_euler = loc, rot
        return c

    th = 0.24 * R  # cookie thickness above its origin plane
    base = plate_top + 0.05 * R
    place("Stack1", (-0.030, 0.030, base), (0.0, 0.0, 0.4))
    place("Stack2", (-0.026, 0.034, base + th * 1.05), (0.05, -0.03, 2.1), s=0.97)
    place("Stack3", (-0.032, 0.028, base + th * 2.1), (-0.04, 0.05, 4.0), s=1.02)
    place("Flat", (0.052, 0.026, base), (0, 0.02, 1.2), s=0.95)
    hero = place("Front", (0.012, -0.046, base + 0.004), (0.06, -0.05, -1.25), s=1.05)

    # crumbs near the bite
    for i in range(26):
        bm = bmesh.new()
        bmesh.ops.create_icosphere(bm, subdivisions=2, radius=1)
        me = bpy.data.meshes.new(f"crumb{i}")
        bm.to_mesh(me)
        bm.free()
        cr = link(bpy.data.objects.new(me.name, me))
        smooth(cr)
        me.materials.append(m_choc if i % 6 == 0 else m_cookie)
        s = rng.uniform(0.0008, 0.0028)
        cr.scale = (s, s * rng.uniform(0.6, 1.2), s * rng.uniform(0.5, 0.9))
        a, d = rng.uniform(-0.6, 1.6), rng.uniform(0.03, 0.075)
        x, y = hero.location.x + math.cos(a) * d, hero.location.y + math.sin(a) * d - 0.01
        rr = math.hypot(x, y)
        z = plate_top if rr < 0.078 else (0.0 if rr > 0.122 else None)
        if z is None:
            x, y, z = x * 1.5, y * 1.5 - 0.02, 0.0
        cr.location = (x, y, z + s * 0.5)
        cr.rotation_euler = (rng.uniform(0, 6),) * 3
        dm = cr.modifiers.new("d", "DISPLACE")
        dm.texture = clouds(me.name + "_t", 0.5)
        dm.strength = 0.5

    # glass of milk in the background
    glass = simple_material("Glass", (1, 1, 1), 0.0, **{"Transmission Weight": 1.0, "IOR": 1.5})
    gnt = glass.node_tree
    lp, tr, mx = node(gnt, "ShaderNodeLightPath"), node(gnt, "ShaderNodeBsdfTransparent"), node(gnt, "ShaderNodeMixShader")
    gnt.links.new(lp.outputs["Is Shadow Ray"], mx.inputs["Fac"])
    gnt.links.new(gnt.nodes["Principled BSDF"].outputs[0], mx.inputs[1])
    gnt.links.new(tr.outputs[0], mx.inputs[2])
    gnt.links.new(mx.outputs[0], gnt.nodes["Material Output"].inputs["Surface"])
    lathe("Glass", [(0.0, 0.0), (0.031, 0.0), (0.036, 0.11)], glass, thickness=0.0025).location = (0.13, 0.17, 0.0005)
    milk = simple_material("Milk", (0.95, 0.93, 0.88), 0.25, **{"Subsurface Weight": 0.9})
    lathe("MilkBody", [(0.0, 0.003), (0.0285, 0.003), (0.0325, 0.085), (0.0, 0.085)], milk).location = (0.13, 0.17, 0.0005)

    # light: low warm sun + soft window + sky fill
    world = bpy.data.worlds.new("World")
    sc.world = world
    world.use_nodes = True
    bg = world.node_tree.nodes["Background"]
    bg.inputs["Color"].default_value = (0.55, 0.68, 1.0, 1)
    bg.inputs["Strength"].default_value = 0.22

    sun = bpy.data.lights.new("Sun", "SUN")
    sun.energy = 9.0
    sun.angle = math.radians(2.5)
    sun.color = (1.0, 0.82, 0.62)
    so = link(bpy.data.objects.new("Sun", sun))
    so.rotation_euler = (math.radians(68), 0, math.radians(118))

    win = bpy.data.lights.new("Window", "AREA")
    win.shape = "RECTANGLE"
    win.size, win.size_y = 0.9, 1.2
    win.energy = 28
    win.color = (1.0, 0.95, 0.88)
    wo = link(bpy.data.objects.new("Window", win))
    wo.location = (-0.9, 0.2, 0.6)
    wo.rotation_euler = (0, math.radians(-62), 0)

    root, pose = build_character(m_cookie, m_choc, rng)
    for f in range(1, TOTAL + 1):
        pose(f)

    fill = bpy.data.lights.new("Fill", "AREA")
    fill.size = 0.5
    fill.energy = 9
    fill.color = (1.0, 0.93, 0.85)
    fo = link(bpy.data.objects.new("Fill", fill))
    fo.location = (0.35, -0.75, 0.3)
    ft = fo.constraints.new("TRACK_TO")
    ft.target = root
    ft.track_axis, ft.up_axis = "TRACK_NEGATIVE_Z", "UP_Y"

    # cameras
    focus = link(bpy.data.objects.new("Focus", None))
    focus.parent = root
    focus.location = (0, -0.012, 0.068)
    centre = link(bpy.data.objects.new("Centre", None))
    centre.location = (0.0, -0.13, 0.04)

    def camera(name, lens, fstop, target):
        cd = bpy.data.cameras.new(name)
        cd.lens = lens
        cd.sensor_width = 36
        cd.clip_start = 0.01
        cd.dof.use_dof = True
        cd.dof.focus_object = focus
        cd.dof.aperture_fstop = fstop
        co = link(bpy.data.objects.new(name, cd))
        t = co.constraints.new("TRACK_TO")
        t.target = target
        t.track_axis, t.up_axis = "TRACK_NEGATIVE_Z", "UP_Y"
        return co

    # shot 1: low wide shot following the walk
    c1 = camera("Cam1", 50, 4.0, focus)
    key(c1, "location", 1, (0.13, -0.66, 0.05))
    key(c1, "location", SHOT, (0.10, -0.60, 0.065))
    # shot 2: close-up, slow push-in
    c2 = camera("Cam2", 85, 4.0, focus)
    key(c2, "location", SHOT + 1, (0.05, -0.50, 0.075))
    key(c2, "location", SHOT * 2, (0.03, -0.45, 0.08))
    # shot 3: high angle, slow pull back
    c3 = camera("Cam3", 42, 5.6, centre)
    key(c3, "location", SHOT * 2 + 1, (0.01, -0.52, 0.24))
    key(c3, "location", TOTAL, (0.02, -0.62, 0.30))

    for i, c in enumerate((c1, c2, c3)):
        mk = sc.timeline_markers.new(f"shot{i + 1}", frame=i * SHOT + 1)
        mk.camera = c
    sc.camera = c1
    sc.frame_start, sc.frame_end = 1, TOTAL


def setup_render(samples, scale):
    sc = bpy.context.scene
    r = sc.render
    r.engine = "CYCLES"
    r.resolution_x, r.resolution_y = 720, 1280
    r.resolution_percentage = scale
    r.fps = FPS
    r.image_settings.file_format = "PNG"
    r.use_persistent_data = True
    c = sc.cycles
    c.device = "CPU"
    c.samples = samples
    c.use_adaptive_sampling = True
    c.adaptive_threshold = 0.04
    c.use_denoising = True
    c.denoiser = "OPENIMAGEDENOISE"
    c.max_bounces = 6
    c.glossy_bounces = 3
    c.transmission_bounces = 6
    c.caustics_reflective = c.caustics_refractive = False
    sc.view_settings.view_transform = "AgX"
    try:
        sc.view_settings.look = "AgX - Medium High Contrast"
    except TypeError:
        pass


def main():
    a = parse()
    build()
    setup_render(a.samples, a.scale)
    sc = bpy.context.scene
    os.makedirs(a.out, exist_ok=True)
    frames = [int(f) for f in a.frames.split(",") if f] if a.frames else range(a.range[0], a.range[1] + 1)
    for f in frames:
        path = os.path.join(a.out, f"f{f:04d}.png")
        if os.path.exists(path):
            continue
        sc.frame_set(f)
        sc.render.filepath = path
        bpy.ops.render.render(write_still=True)


if __name__ == "__main__":
    main()
