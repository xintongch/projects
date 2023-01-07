import { Outlet, Link } from "react-router-dom";

const Header = () => {
    return (
        <div className="container-fluid d-flex justify-content-end my-2 w-100">
            <div style={hs1} className="d-inline-block hover-pointer rounded-4"><Link to="/" className="text-decoration-none text-dark">Search</Link></div>
        <div style={hs1} className="d-inline-block hover-pointer rounded-4 mr-4"><Link className="text-decoration-none text-dark" to="/bookings">My Bookings</Link></div>
        </div>
    )
}

const hs1 = {
    'padding': '8px 8px',
    'fontSize': '15px',
    'cursor': 'pointer',
    'border':'2px black solid'
}

export default Header