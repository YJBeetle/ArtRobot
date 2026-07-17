use std::fmt::{Display, Formatter};

/// Errors produced while loading assets or encoding renderer output.
#[derive(Debug)]
pub enum Error {
    Cairo(cairo::Error),
    Image(image::ImageError),
    Io(std::io::Error),
    InvalidInput(String),
    Svg(String),
}

impl Display for Error {
    fn fmt(&self, formatter: &mut Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Cairo(error) => write!(formatter, "Cairo error: {error}"),
            Self::Image(error) => write!(formatter, "image error: {error}"),
            Self::Io(error) => write!(formatter, "I/O error: {error}"),
            Self::InvalidInput(message) => formatter.write_str(message),
            Self::Svg(message) => write!(formatter, "SVG error: {message}"),
        }
    }
}

impl std::error::Error for Error {}

impl From<cairo::Error> for Error {
    fn from(error: cairo::Error) -> Self {
        Self::Cairo(error)
    }
}

impl From<image::ImageError> for Error {
    fn from(error: image::ImageError) -> Self {
        Self::Image(error)
    }
}

impl From<std::io::Error> for Error {
    fn from(error: std::io::Error) -> Self {
        Self::Io(error)
    }
}

pub type Result<T> = std::result::Result<T, Error>;
