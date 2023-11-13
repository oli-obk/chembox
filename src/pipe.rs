use spanned::Spanned;

pub const PIPES: &[&str] = &[
    "pipe0.png",
    "pipe1.png",
    "pipe2.png",
    "pipe2opposite.png",
    "pipe3.png",
    "pipe4.png",
    "drain_pipe0.png",
    "drain_pipe1.png",
    "drain_pipe2.png",
    "drain_pipe2opposite.png",
    "drain_pipe3.png",
    "drain_pipe4.png",
    "pump.png",
];

#[derive(Default, Debug)]
pub struct Pipe {
    // Bitmask of the actual data
    data: u8,
}

pub enum Rotation {
    /// 0°
    Zero,
    /// 90°
    Ninety,
    /// 180°
    OneEighty,
    /// 270°
    TwoSeventy,
}

#[derive(Clone, Copy)]
pub enum PipeTile {
    Empty = 0,
    One = 1,
    Turn = 2,
    Straight = 3,
    T = 4,
    All = 5,
}

impl PipeTile {
    pub fn index(self, drain: bool) -> u8 {
        self as u8 + 6 * drain as u8
    }
}

impl Pipe {
    pub const EMPTY: Self = Self { data: 0 };
    pub fn tile_and_rotation(&self) -> (PipeTile, bool, Rotation) {
        let drain = ((self.data & 0b1100_0000) >> 6) != 0;
        let (tile, rot) = match self.data & 0b0000_1111 {
            0b0000 => (PipeTile::Empty, Rotation::Zero),

            0b1000 => (PipeTile::One, Rotation::Zero),
            0b0100 => (PipeTile::One, Rotation::Ninety),
            0b0010 => (PipeTile::One, Rotation::OneEighty),
            0b0001 => (PipeTile::One, Rotation::TwoSeventy),

            0b1100 => (PipeTile::Turn, Rotation::Zero),
            0b0110 => (PipeTile::Turn, Rotation::Ninety),
            0b0011 => (PipeTile::Turn, Rotation::OneEighty),
            0b1001 => (PipeTile::Turn, Rotation::TwoSeventy),

            0b1010 => (PipeTile::Straight, Rotation::Zero),
            0b0101 => (PipeTile::Straight, Rotation::Ninety),

            0b1110 => (PipeTile::T, Rotation::Zero),
            0b0111 => (PipeTile::T, Rotation::Ninety),
            0b1011 => (PipeTile::T, Rotation::OneEighty),
            0b1101 => (PipeTile::T, Rotation::TwoSeventy),

            0b1111 => (PipeTile::All, Rotation::Zero),

            _ => unreachable!(),
        };
        (tile, drain, rot)
    }

    pub fn set_dirs(&mut self, dir: Directions<bool>) {
        self.data &= 0b1111_0000;
        if dir.up {
            self.data |= 0b1000;
        }
        if dir.right {
            self.data |= 0b0100;
        }
        if dir.down {
            self.data |= 0b0010;
        }
        if dir.left {
            self.data |= 0b0001;
        }
    }

    /// (drain, connections)
    pub fn parts(&self) -> (bool, Directions<bool>) {
        let drain = (self.data & 0b1000_0000) != 0;
        let directions = Directions {
            up: self.data & 0b0000_1000 != 0,
            right: self.data & 0b0000_0100 != 0,
            down: self.data & 0b0000_0010 != 0,
            left: self.data & 0b0000_0001 != 0,
        };
        (drain, directions)
    }

    pub fn parse(c: &Spanned<char>) -> Option<Self> {
        Some(Self {
            data: match **c {
                ' ' => 0,

                '▩' => 0b0100_0000,

                '╨' => 0b1000,
                '╞' => 0b0100,
                '╥' => 0b0010,
                '╡' => 0b0001,

                '╚' => 0b1100,
                '╔' => 0b0110,
                '╗' => 0b0011,
                '╝' => 0b1001,

                '║' => 0b1010,
                '═' => 0b0101,

                '╠' => 0b1110,
                '╦' => 0b0111,
                '╣' => 0b1011,
                '╩' => 0b1101,

                '╬' => 0b1111,

                _ => return None,
            },
        })
    }
}

#[derive(Default, Clone, Copy, Debug)]
pub struct Directions<T> {
    pub up: T,
    pub down: T,
    pub left: T,
    pub right: T,
}

impl<T> Directions<T> {
    pub fn iter(&self) -> impl Iterator<Item = &T> {
        [&self.up, &self.down, &self.left, &self.right].into_iter()
    }
}
