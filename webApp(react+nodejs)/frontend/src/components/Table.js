const Table = ({ data }) => {
    // const index = 1;
    return (
        <div id='results' style={s0} className="container">
            <div className="mx-auto rounded-4 text-danger bg-white col-sm-11 col-md-3 h5" id="no-result">
                No results available
            </div>
            <div className="container col-md-10 col-sm-12 pl-sm-2 px-0 mt-4" id="rt-container">
                <table className="table-wo-bottom bg-white rounded-4 mb-4 col-md-12 mx-auto">
                    <thead>
                        <tr className="border-bottom aligh-top">
                            <th className="px-2">#</th>
                            <th className="px-2">Image</th>
                            <th className="px-2">Business Name</th>
                            <th className="px-2">Rating</th>
                            <th className="px-3">Distance (miles)</th>
                        </tr>
                    </thead>
                    <tbody>
                        {!data ? "" : data.map(
                            (bus, index) =>
                                <tr>
                                    <td className="px-2">{index + 1}</td>
                                    <td className="px-2"><img src={bus.image_url} style={s} alt=""></img></td>
                                    <td className="px-2">{bus.name}</td>
                                    <td className="px-2">{bus.rating}</td>
                                    <td className="px-2">{bus.distance}</td>
                                </tr>
                        )}
                    </tbody>
                </table>
            </div>
        </div>

        // <div>{data.businesses.map(bus =>
        //     <div>{bus.id}</div>)}</div>
        // <div>hi
        //     {data.map(bus =>
        //         <div>{bus.id}</div> )}
        // </div>
    )
}

const s = {
    'width': '100px',
    'heigth': '100px'
}
const s0 = {
    'display':'none'
}
// const s1 = {
//     'width':'1000px'
// }

export default Table;