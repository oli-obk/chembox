use spanned::Spanned;

pub const PIPES: &[&str] = &[
    "end_pipe0.png",
    "end_pipe1.png",
    "end_pipe2.png",
    "end_pipe2opposite.png",
    "end_pipe3.png",
    "end_pipe4.png",
    "drain_pipe0.png",
    "drain_pipe1.png",
    "drain_pipe2.png",
    "drain_pipe2opposite.png",
    "drain_pipe3.png",
    "drain_pipe4.png",
    "pipe0.png",
    "pipe1.png",
    "pipe2.png",
    "pipe2opposite.png",
    "pipe3.png",
    "pipe4.png",
    "pump.png",
];

#[derive(Default)]
pub struct Pipe {
    // Bitmask of the actual data
    data: u8,
}

pub enum Center {
    Nothing,
    Drain,
    Connected,
}

impl Pipe {
    pub const EMPTY: Self = Self { data: 0 };
    pub fn index_and_rotation(&self) -> (u8, u8) {
        let i = ((self.data & 0b1100_0000) >> 6) * 6;
        let (j, rot) = match self.data & 0b0000_1111 {
            0b0000 => (0, 0),

            0b1000 => (1, 0),
            0b0100 => (1, 1),
            0b0010 => (1, 2),
            0b0001 => (1, 3),

            0b1100 => (2, 0),
            0b0110 => (2, 1),
            0b0011 => (2, 2),
            0b1001 => (2, 3),

            0b1010 => (3, 0),
            0b0101 => (3, 1),

            0b1110 => (4, 0),
            0b0111 => (4, 1),
            0b1011 => (4, 2),
            0b1101 => (4, 3),

            0b1111 => (5, 0),

            _ => unreachable!(),
        };
        (i + j, rot)
    }

    pub fn parts(&self) -> (Center, Directions<bool>) {
        let center = match (self.data & 0b1100_0000) >> 6 {
            0 => Center::Nothing,
            1 => Center::Drain,
            2 => Center::Connected,
            _ => unreachable!(),
        };
        let directions = Directions {
            up: self.data & 0b0000_1000 != 0,
            right: self.data & 0b0000_0100 != 0,
            down: self.data & 0b0000_0010 != 0,
            left: self.data & 0b0000_0001 != 0,
        };
        (center, directions)
    }

    pub fn parse(c: &Spanned<char>) -> Option<Self> {
        Some(Self {
            data: match **c {
                ' ' => 0,

                '▩' => 0b0100_0000,

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

pub struct Directions<T> {
    up: T,
    down: T,
    left: T,
    right: T,
}

impl<T> Directions<T> {
    pub fn iter(&self) -> impl Iterator<Item = &T> {
        [&self.up, &self.down, &self.left, &self.right].into_iter()
    }
}
