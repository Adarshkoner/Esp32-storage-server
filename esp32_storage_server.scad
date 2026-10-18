
// ESP32 Storage Server enclosure — parametric prototype
// Units: mm. Verify all component footprints and connector locations before printing.
$fn = 64;

part = "base"; // "base", "lid", or "assembly"
L = 110;
W = 85;
H = 45;
wall = 2.4;
floor_t = 2.5;
lid_t = 3;
corner_r = 5;
fit_clearance = 0.35; // estimated FDM clearance; tune for printer/material

body_h = H - lid_t; // 42
inner_L = L - 2*wall;
inner_W = W - 2*wall;

// Mounting coordinates are estimates based on supplied board/module overall sizes.
// ESP32: 65 x 25; assumed four-hole pattern 58 x 18 mm (NOT verified).
// Fan: 40 x 40 x 10; assumed mounting hole pitch 32 mm.
// Battery holder: assumed single-cell holder approx. 82 x 24 mm.
// SD module: 50 x 10; TP4056 module: 30 x 5.

module rounded_box(x,y,z,r) {
    hull() {
        for (xx=[r,x-r], yy=[r,y-r])
            translate([xx,yy,0]) cylinder(r=r,h=z);
    }
}

module base_shell() {
    difference() {
        rounded_box(L,W,body_h,corner_r);
        translate([wall,wall,floor_t])
            rounded_box(L-2*wall,W-2*wall,body_h-floor_t+0.1,max(1,corner_r-wall));
        // Front panel: USB-C opening for TP4056 charging port (position estimate)
        translate([L/2-5.5,-0.1,10])
            cube([11,wall+0.2,4.5]);
        // Front panel: microSD card access slot (position estimate; tune to actual slot)
        translate([L-60,-0.1,17])
            cube([16,wall+0.2,3]);
        // Right side cable/USB access opening (optional service access)
        translate([L-wall-0.1,W/2-6,10])
            cube([wall+0.2,12,6]);
        // Side ventilation slots on both long side walls
        for (z0=[12:7:30]) {
            translate([20,-0.1,z0]) cube([24,wall+0.2,2.2]);
            translate([66,-0.1,z0]) cube([24,wall+0.2,2.2]);
            translate([20,W-wall-0.1,z0]) cube([24,wall+0.2,2.2]);
            translate([66,W-wall-0.1,z0]) cube([24,wall+0.2,2.2]);
        }
    }

    // Lid screw bosses / posts in base, with pilot holes
    for (x=[7,L-7], y=[7,W-7]) {
        difference() {
            translate([x,y,floor_t]) cylinder(d=8,h=body_h-floor_t-1);
            translate([x,y,floor_t-0.1]) cylinder(d=2.7,h=body_h-floor_t+1);
        }
    }

    // ESP32 standoffs: approximate 58 x 18 mm hole pattern
    for (x=[L/2-29,L/2+29], y=[W/2-9,W/2+9]) {
        difference() {
            translate([x,y,floor_t]) cylinder(d=6,h=5);
            translate([x,y,floor_t-0.1]) cylinder(d=2.2,h=5.2);
        }
    }

    // Battery holder retaining rails (not a battery electrical enclosure)
    translate([10,12,floor_t]) cube([3,25,4]);
    translate([10,12+25,floor_t]) cube([82,3,4]);
    translate([10+82,12,floor_t]) cube([3,28,4]);

    // SD module support rails; approximate 50 x 10 mm module
    translate([L-59,W-24,floor_t]) cube([50,2,3]);
    translate([L-59,W-12,floor_t]) cube([50,2,3]);

    // TP4056 support pads; connector cutout position is an estimate
    translate([L/2-15,7,floor_t]) cube([30,5,3]);
}

module lid() {
    difference() {
        rounded_box(L,W,lid_t,corner_r);
        // Fan airflow aperture; centered toward rear to reduce overlap with ESP32
        translate([L/2,W-25,-0.1]) cylinder(d=34,h=lid_t+0.2);
        // Fan screw holes, assumed 32 mm spacing
        for (x=[L/2-16,L/2+16], y=[W-25-16,W-25+16])
            translate([x,y,-0.1]) cylinder(d=3.4,h=lid_t+0.2);
        // Perimeter ventilation slots
        for (x=[L/2-28:5:L/2+28])
            translate([x,8,-0.1]) cube([2,10,lid_t+0.2]);
        // Lid corner screw holes aligned with bosses
        for (x=[7,L-7], y=[7,W-7])
            translate([x,y,-0.1]) cylinder(d=3.4,h=lid_t+0.2);
    }
}

if (part == "base") base_shell();
else if (part == "lid") lid();
else {
    color("dimgray") base_shell();
    color("silver") translate([0,0,body_h+3]) lid();
}
