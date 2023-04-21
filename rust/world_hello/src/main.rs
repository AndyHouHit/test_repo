fn _greet_world() {
    let southern_germany = "Grüß Gott!";
    let chinese = "世界，你好, 中国";
    let english = "World, hello";
    let regions = [southern_germany, chinese, english];
    for region in regions.iter() {
        println!("{}", &region);
    }
}

const _MAX_POINT: u32 = 1000_1000;

fn _var_test() {
    let mut x = 5;
    println!("Value X is {}", x);
    x = _MAX_POINT;
    println!("Value X is {}", x);

    // unused variable need be _ prefix
    let _y = 10;

    let (a, mut b): (bool, bool) = (true, false);
    println!("a {}, b {}", a, b);
    b = true;
    println!("a {}, b {}", a, b);

    let a = 5;
    let a = a + 1;
    {
        let a = 2 * a;
        println!("a in sub is {}", a);
    }
    println!("a in glob is {}", a);

    let a: u8 = 255;
    let b = a.wrapping_add(20);
    println!("b is {}, size is {}", b, std::mem::size_of_val(&b));

}

fn _num_test()
{
    for i in 1..5 {
        println!("{}", i);
    }
    for i in 'a'..='z' {
        println!("{}, size is {}", i, std::mem::size_of_val(&i));
    }
    let a = "this is my book";
    println!("Sizeof {} is {}", a, std::mem::size_of_val(&a));
}

fn _ret_fn(x:i32, y :i32) -> i32 {
    let x = x + 1;
    let y = y * 2;
    //return x + y;
    x + y
}

fn _fn_test() {
    let y = {
        let x = 1;
        x * 2 + 10
    };
    println!("Y is {}", y)
}

fn _fn_test1(x:i32) -> i32 {
    println!("x is {}", x);
    return x;
}

fn _str_test() 
{
    let mut s = String::from("hello");
    //let mut s = "hello";
    s.push_str(", world!");
    let s1 = s;
    println!("{}",s1);
}

fn _take_owership(s:String) -> String {
    println!("{}", s);
    return s;
}
fn _make_copy(i: i32) {
    println!("{}", i);
}

fn _copy_test() {
    let s = String::from("Hello");
    let s = _take_owership(s);
    println!("After S is {}", s);
    let x = 5;
    _make_copy(5);
    println!("After x is {}", x);
}

fn _ref_str(s: &mut String) ->usize {
    s.push_str(", Andy");
    return s.len();
}
fn _ref_test() {
    let x = 5;
    let y = &x;
    println!("p{}", y);
    assert_eq!(x, 5);
    assert_eq!(*y, 5);

    let mut x = String::from("Hello");
    let y = _ref_str(&mut x);
    println!("x {}, size {}", x, y);


    let z = &mut x;
    z.push_str("See you");
    println!("z {}",z);
    println!("x {}", x);
    
    let a = & x;
    let b = & x;
    println!("a {}, \nb {}", a, b);
}

fn ref_test2() -> String
{
    let s = String::from("Hello");

    return s;
}


fn main() {
    //_greet_world();
    //_var_test();
    //_num_test();
    //println!("Value is {}", _ret_fn(5, 3));
    //_fn_test();
    //_str_test();
    //_copy_test();

    _ref_test();

    let y = ref_test2();
    println!("{}", y);
}
