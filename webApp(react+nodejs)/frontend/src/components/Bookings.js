import Header from "./Header"

const Bookings = () => {
    return (
        <div className="contianer">
            <Header />
            <div className="clear-both container-fluid text-center px-0" style={s0}>
                <div className="mt-2 w-100 container-fluid px-0 mx-0" id="no-booking">
                    <div className="mx-auto text-danger px-0 mx-0 bg-white col-md-3 col-12 rounded-4 h5 mt-5 py-0"
                        style={s}>
                        No reservations to show
                    </div>
                </div>
            </div>
        </div>
    )
}

const s0 = {
    'height': '100%'
}
const s = {
    "font-size": "24px;"
}
export default Bookings